// Standalone OCPU/1 processor-temperature bridge. This node never consumes or
// republishes the IMU temperature field.
#include <atomic>
#include <chrono>
#include <mutex>
#include <cmath>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/temperature.hpp>

#include "orbvi_sdk/client.hpp"
#include "orbvi_sdk/temperature_stream.hpp"

namespace {
class TemperatureNode final : public rclcpp::Node {
 public:
  TemperatureNode() : Node("orbvi_temperature") {
    bind_address_ =
        declare_parameter<std::string>("bind_address", "0.0.0.0");
    port_ = declare_parameter<std::int64_t>("port", 5620);
    samples_ = declare_parameter<std::int64_t>("samples", 10);
    idle_timeout_sec_ = declare_parameter<double>("idle_timeout_sec", 30.0);
    topic_ = declare_parameter<std::string>("topic", "/orbvi/raw/temperature");
    source_ = declare_parameter<std::string>("source", "cpu");
  }

  int exit_code() const { return timed_out_.load() ? 1 : 0; }

  bool initialize(std::string& error) {
    if (bind_address_.empty() || port_ < 1 || port_ > 65535 || samples_ < 0 ||
        samples_ > 100000 || !std::isfinite(idle_timeout_sec_) ||
        idle_timeout_sec_ <= 0.0 || topic_.empty() || source_.empty()) {
      error =
          "requires nonempty IPv4 bind_address, port 1..65535, samples "
          "0..100000, finite positive idle_timeout_sec, and nonempty topic/source";
      return false;
    }

    orbvi_sdk::TemperatureStreamConfig config;
    config.bind_address = bind_address_;
    config.port = static_cast<std::uint16_t>(port_);
    config.samples = static_cast<std::uint32_t>(samples_);
    client_ = std::make_unique<orbvi_sdk::Client>(
        orbvi_sdk::make_temperature_stream_client(config));

    const auto opened = client_->open();
    if (!opened.ok()) {
      error = "temperature SDK open failed: " + opened.to_string();
      return false;
    }

    orbvi_sdk::CallbackSet callbacks;
    callbacks.on_temperature = [this](const orbvi_sdk::TemperatureSample& sample) {
      publish_sample(sample);
    };
    auto subscription = client_->subscribe(
        orbvi_sdk::SubscribeOptions{8U, orbvi_sdk::DropPolicy::drop_oldest},
        std::move(callbacks));
    if (!subscription.ok()) {
      error = "temperature SDK subscribe failed: " +
              subscription.status().to_string();
      static_cast<void>(client_->close());
      return false;
    }
    subscription_ = std::move(subscription.value());

    publisher_ = create_publisher<sensor_msgs::msg::Temperature>(
        topic_, rclcpp::SensorDataQoS());
    {
      const std::lock_guard<std::mutex> lock(progress_mutex_);
      last_sample_at_ = std::chrono::steady_clock::now();
    }
    // Finite sessions fail after a bounded gap, including when no packet arrives.
    // samples=0 explicitly selects continuous reception without an idle deadline.
    if (samples_ > 0) {
      timer_ = create_wall_timer(std::chrono::milliseconds(100),
                                 [this]() { check_idle(); });
    }
    const auto started = client_->start();
    if (!started.ok()) {
      error = "temperature SDK start failed: " + started.to_string();
      subscription_.cancel();
      static_cast<void>(client_->close());
      return false;
    }

    RCLCPP_INFO(get_logger(),
                "listening for OCPU/1 on %s:%ld, samples=%ld; source=%s; "
                "not IMU temperature",
                bind_address_.c_str(), static_cast<long>(port_),
                static_cast<long>(samples_), source_.c_str());
    return true;
  }

 private:
  void check_idle() {
    if (shutdown_requested_.load()) {
      return;
    }
    std::uint64_t delivered;
    double idle;
    {
      const std::lock_guard<std::mutex> lock(progress_mutex_);
      delivered = published_;
      idle = std::chrono::duration<double>(std::chrono::steady_clock::now() -
                                           last_sample_at_).count();
    }
    if (delivered < static_cast<std::uint64_t>(samples_) &&
        idle >= idle_timeout_sec_ && !shutdown_requested_.exchange(true)) {
      timed_out_.store(true);
      RCLCPP_ERROR(get_logger(),
                   "OCPU idle timeout: received %lu/%ld samples; finite demo failed",
                   static_cast<unsigned long>(delivered), static_cast<long>(samples_));
      rclcpp::shutdown();
    }
  }

  void publish_sample(const orbvi_sdk::TemperatureSample& sample) {
    // The SDK already validates OCPU magic/version/type/reserved/CRC/length and
    // finite values. Keep a defensive finite check here as well.
    if (!std::isfinite(sample.celsius)) {
      return;
    }

    sensor_msgs::msg::Temperature message;
    // Board CLOCK_MONOTONIC cannot be interpreted as ROS wall time. The message
    // stamp is host receive time; monotonic_ns is intentionally metadata-only.
    message.header.stamp = now();
    message.header.frame_id = source_;
    message.temperature = static_cast<double>(sample.celsius);
    message.variance = 0.0;  // sensor_msgs/Temperature: 0 means variance unknown.
    publisher_->publish(message);

    std::uint64_t delivered;
    {
      const std::lock_guard<std::mutex> lock(progress_mutex_);
      last_sample_at_ = std::chrono::steady_clock::now();
      delivered = ++published_;
    }
    if (samples_ > 0 &&
        delivered >= static_cast<std::uint64_t>(samples_) &&
        !shutdown_requested_.exchange(true)) {
      rclcpp::shutdown();
    }
  }

  std::string bind_address_;
  std::string topic_;
  std::string source_;
  std::int64_t port_{};
  std::int64_t samples_{};
  double idle_timeout_sec_{};
  std::mutex progress_mutex_;
  std::chrono::steady_clock::time_point last_sample_at_{};
  std::uint64_t published_{0U};
  rclcpp::TimerBase::SharedPtr timer_;
  std::unique_ptr<orbvi_sdk::Client> client_;
  orbvi_sdk::SubscriptionHandle subscription_;
  rclcpp::Publisher<sensor_msgs::msg::Temperature>::SharedPtr publisher_;
  std::atomic<bool> shutdown_requested_{false};
  std::atomic<bool> timed_out_{false};
};
}  // namespace

int main(int argc, char** argv) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<TemperatureNode>();
  std::string error;
  if (!node->initialize(error)) {
    RCLCPP_FATAL(node->get_logger(), "%s", error.c_str());
    rclcpp::shutdown();
    return 1;
  }
  rclcpp::spin(node);
  const int exit_code = node->exit_code();
  rclcpp::shutdown();
  return exit_code;
}
