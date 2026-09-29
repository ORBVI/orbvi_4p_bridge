#include <array>
#include <atomic>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <limits>
#include <memory>
#include <string>
#include <utility>

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <sensor_msgs/msg/imu.hpp>

#include "orbvi_sdk/client.hpp"
#include "orbvi_sdk/fake.hpp"
#include "orbvi_sdk/rtsp.hpp"
#include "orbvi_sdk/udp_imu.hpp"

namespace {

constexpr std::size_t kCameraCount = 4U;

class BridgeNode final : public rclcpp::Node {
 public:
  BridgeNode() : Node("orbvi_4p_bridge") {
    runtime_mode_ = declare_parameter<std::string>("runtime_mode", "unsupported");
    rtsp_urls_[0] = declare_parameter<std::string>("rtsp_url", "");
    for (std::size_t i = 1; i < kCameraCount; ++i)
      rtsp_urls_[i] = declare_parameter<std::string>("rtsp_url_" + std::to_string(i), "");
    imu_enabled_ = declare_parameter<bool>("imu_enabled", false);
    imu_bind_address_ = declare_parameter<std::string>("imu_bind_address", "0.0.0.0");
    imu_source_address_ = declare_parameter<std::string>("imu_source_address", "");
    imu_port_ = declare_parameter<std::int64_t>("imu_port", 5600);
    imu_frame_id_ = declare_parameter<std::string>("imu_frame_id", "orbvi_imu");
    camera_count_ = declare_parameter<std::int64_t>("camera_count", 4);
    frames_per_camera_ = declare_parameter<std::int64_t>("frames_per_camera", 300);
    emit_interval_us_ = declare_parameter<std::int64_t>("emit_interval_us", 33333);
    frame_width_ = declare_parameter<std::int64_t>("frame_width", 1280);
    frame_height_ = declare_parameter<std::int64_t>("frame_height", 1088);
    frame_stride_ = declare_parameter<std::int64_t>("frame_stride", 0);
    callback_queue_capacity_ =
        declare_parameter<std::int64_t>("callback_queue_capacity", 8);
    subscription_queue_capacity_ =
        declare_parameter<std::int64_t>("subscription_queue_capacity", 8);
    publish_queue_depth_ = declare_parameter<std::int64_t>("publish_queue_depth", 5);
    topic_namespace_ = declare_parameter<std::string>("topic_namespace", "/orbvi/raw");
    frame_id_prefix_ = declare_parameter<std::string>("frame_id_prefix", "orbvi_camera_");
  }

  ~BridgeNode() override { shutdown(); }

  bool initialize(std::string& error) {
    if (runtime_mode_ != "fake" && runtime_mode_ != "rtsp" &&
        runtime_mode_ != "udp_imu") {
      error = "runtime_mode must be explicitly fake, rtsp or udp_imu";
      return false;
    }
    if (runtime_mode_ == "rtsp") {
      if (camera_count_ < 1 || camera_count_ > static_cast<std::int64_t>(kCameraCount)) {
        error = "rtsp mode requires camera_count=1..4";
        return false;
      }
      for (std::size_t i = 0; i < kCameraCount; ++i) {
        const bool configured = i < static_cast<std::size_t>(camera_count_);
        const bool valid = rtsp_urls_[i].rfind("rtsp://", 0) == 0 ||
                           rtsp_urls_[i].rfind("rtsps://", 0) == 0;
        if ((configured && !valid) || (!configured && !rtsp_urls_[i].empty())) {
          error = "rtsp mode requires exactly camera_count valid explicit rtsp_url[_1..3] URLs";
          return false;
        }
      }
    } else if (runtime_mode_ == "fake") {
      if (camera_count_ != static_cast<std::int64_t>(kCameraCount)) {
        error = "fake mode requires camera_count=4";
        return false;
      }
      if (frames_per_camera_ <= 0 || emit_interval_us_ <= 0 ||
          frame_width_ <= 0 || frame_height_ <= 0) {
        error = "fake frame counts, interval and dimensions must be positive";
        return false;
      }
      if (frame_width_ > static_cast<std::int64_t>(std::numeric_limits<std::uint32_t>::max()) ||
          frame_height_ > static_cast<std::int64_t>(std::numeric_limits<std::uint32_t>::max())) {
        error = "frame dimensions exceed the SDK uint32 contract";
        return false;
      }
      const std::int64_t resolved_stride =
          frame_stride_ <= 0 ? frame_width_ : frame_stride_;
      if (resolved_stride != frame_width_) {
        error = "Fake frames are packed; frame_stride must be 0 or equal frame_width";
        return false;
      }
    }
    if (runtime_mode_ == "fake" && imu_enabled_) {
      error = "fake mode cannot enable real UDP IMU";
      return false;
    }
    if (runtime_mode_ == "udp_imu" && !imu_enabled_) {
      error = "udp_imu mode requires imu_enabled=true";
      return false;
    }
    if (imu_enabled_ && (imu_frame_id_.empty() || imu_bind_address_.empty() ||
                         imu_port_ <= 0 || imu_port_ > 65535)) {
      error = "IMU requires a nonempty frame_id/bind_address and port 1..65535";
      return false;
    }
    if (callback_queue_capacity_ <= 0 || subscription_queue_capacity_ <= 0 ||
        publish_queue_depth_ <= 0) {
      error = "queue capacities must be greater than zero";
      return false;
    }
    if (frame_id_prefix_.empty()) {
      error = "frame_id_prefix must not be empty";
      return false;
    }
    if (topic_namespace_.empty() || topic_namespace_.front() != '/') {
      error = "topic_namespace must be an absolute ROS namespace such as /orbvi/raw";
      return false;
    }
    while (topic_namespace_.size() > 1U && topic_namespace_.back() == '/') {
      topic_namespace_.pop_back();
    }

    try {
      create_publishers();
      if (imu_enabled_) {
        imu_publisher_ = create_publisher<sensor_msgs::msg::Imu>(
            topic_namespace_ + "/imu", sensor_qos());
        orbvi_sdk::UdpImuConfig imu_config;
        imu_config.bind_address = imu_bind_address_;
        imu_config.source_address = imu_source_address_;
        imu_config.port = static_cast<std::uint16_t>(imu_port_);
        imu_client_ = orbvi_sdk::make_udp_imu_client(imu_config);
        orbvi_sdk::CallbackSet imu_callbacks;
        imu_callbacks.on_imu = [this](const orbvi_sdk::ImuEvent& event) {
          publish_imu(event);
        };
        auto imu_subscription = imu_client_.subscribe(
            orbvi_sdk::SubscribeOptions{
                static_cast<std::size_t>(subscription_queue_capacity_),
                orbvi_sdk::DropPolicy::drop_oldest}, std::move(imu_callbacks));
        if (!imu_subscription.ok()) {
          error = "IMU SDK subscribe failed: " + imu_subscription.status().to_string();
          shutdown();
          return false;
        }
        imu_subscription_ = std::move(imu_subscription).value();
        auto status = imu_client_.open();
        if (!status.ok()) {
          error = "IMU SDK open failed: " + status.to_string();
          shutdown();
          return false;
        }
        status = imu_client_.start();
        if (!status.ok()) {
          error = "IMU SDK start failed: " + status.to_string();
          shutdown();
          return false;
        }
      }

      if (runtime_mode_ == "udp_imu") {
        RCLCPP_INFO(get_logger(), "UDP IMU -> %s/imu", topic_namespace_.c_str());
        return true;
      }
      if (runtime_mode_ == "fake") {
        orbvi_sdk::FakeConfig config =
            orbvi_sdk::make_default_fake_config(static_cast<std::uint32_t>(kCameraCount));
        config.frames_per_camera = static_cast<std::uint64_t>(frames_per_camera_);
        config.imu_samples = 0U;
        config.frame_width = static_cast<std::uint32_t>(frame_width_);
        config.frame_height = static_cast<std::uint32_t>(frame_height_);
        config.frame_stride = static_cast<std::uint32_t>(
            frame_stride_ <= 0 ? frame_width_ : frame_stride_);
        config.emit_interval_us = static_cast<std::uint32_t>(emit_interval_us_);
        config.emit_frame_sync = false;
        config.emit_timing_health = false;
        config.auto_emit = true;
        config.fault = orbvi_sdk::FakeFault::none;
        client_ = orbvi_sdk::make_fake_client(config);
      } else {
        orbvi_sdk::RtspConfig config;
        config.urls = rtsp_urls_;
        client_ = orbvi_sdk::make_rtsp_client(config);
      }

      orbvi_sdk::CallbackSet callbacks;
      callbacks.on_frame = [this](const orbvi_sdk::FrameView& view) {
        publish_frame(view);
      };

      auto subscription = client_.subscribe(
          orbvi_sdk::SubscribeOptions{
              static_cast<std::size_t>(subscription_queue_capacity_),
              orbvi_sdk::DropPolicy::drop_oldest},
          std::move(callbacks));
      if (!subscription.ok()) {
        error = "SDK subscribe failed: " + subscription.status().to_string();
        shutdown();
        return false;
      }
      subscription_ = std::move(subscription).value();

      const orbvi_sdk::Status open_status = client_.open();
      if (!open_status.ok()) {
        error = "SDK open failed: " + open_status.to_string();
        shutdown();
        return false;
      }

      const orbvi_sdk::Status start_status = client_.start();
      if (!start_status.ok()) {
        error = "SDK start failed: " + start_status.to_string();
        shutdown();
        return false;
      }
    } catch (const std::exception& exception) {
      error = std::string{"SDK initialization threw: "} + exception.what();
      shutdown();
      return false;
    }

    if (runtime_mode_ == "fake") {
      RCLCPP_WARN(get_logger(),
                  "runtime_mode=fake is test-only: synthetic mono8 images; "
                  "Header.stamp uses host ROS clock, not sensor hard sync");
      RCLCPP_INFO(get_logger(), "publishing camera_0..camera_3 on %s/camera_<index>/image",
                  topic_namespace_.c_str());
    } else if (runtime_mode_ == "rtsp") {
      // Do not log RTSP URLs: they may contain credentials.
      RCLCPP_INFO(get_logger(), "runtime_mode=rtsp: publishing real camera_0..camera_%lld on %s/camera_<index>/image",
                  static_cast<long long>(camera_count_ - 1), topic_namespace_.c_str());
    }
    if (imu_enabled_) {
      RCLCPP_INFO(get_logger(), "UDP IMU -> %s/imu", topic_namespace_.c_str());
    }
    return true;
  }

  void shutdown() noexcept {
    bool expected = false;
    if (!shutdown_requested_.compare_exchange_strong(expected, true)) {
      return;
    }

    try {
      subscription_.cancel();
    } catch (...) {
    }
    try {
      static_cast<void>(client_.stop());
    } catch (...) {
    }
    try {
      static_cast<void>(client_.close());
    } catch (...) {
    }
    try {
      imu_subscription_.cancel();
    } catch (...) {
    }
    try {
      static_cast<void>(imu_client_.stop());
    } catch (...) {
    }
    try {
      static_cast<void>(imu_client_.close());
    } catch (...) {
    }
  }

 private:
  rclcpp::QoS sensor_qos() const {
    return
        rclcpp::QoS(rclcpp::KeepLast(static_cast<std::size_t>(publish_queue_depth_)))
            .best_effort()
            .durability_volatile();
  }

  void create_publishers() {
    const rclcpp::QoS qos = sensor_qos();
    for (std::size_t index = 0U; index < static_cast<std::size_t>(camera_count_); ++index) {
      const std::string topic =
          topic_namespace_ + "/camera_" + std::to_string(index) + "/image";
      publishers_[index] = create_publisher<sensor_msgs::msg::Image>(topic, qos);
    }
  }

  void publish_imu(const orbvi_sdk::ImuEvent& event) {
    const auto& sample = event.sample;
    // SDK receive time is host CLOCK_REALTIME; board/IIO timestamps are not Unix time.
    constexpr std::int64_t kBillion = 1000000000LL;
    const auto seconds = sample.timestamp_ns / kBillion;
    if (!sample.crc_valid || sample.timestamp_ns <= 0 ||
        seconds > std::numeric_limits<std::int32_t>::max() ||
        !std::isfinite(sample.accel_x_mps2) ||
        !std::isfinite(sample.accel_y_mps2) ||
        !std::isfinite(sample.accel_z_mps2) ||
        !std::isfinite(sample.gyro_x_radps) ||
        !std::isfinite(sample.gyro_y_radps) ||
        !std::isfinite(sample.gyro_z_radps)) {
      RCLCPP_WARN_THROTTLE(get_logger(), *get_clock(), 1000, "dropping invalid IMU sample");
      return;
    }
    auto message = sensor_msgs::msg::Imu();
    message.header.stamp.sec = static_cast<std::int32_t>(seconds);
    message.header.stamp.nanosec = static_cast<std::uint32_t>(sample.timestamp_ns % kBillion);
    message.header.frame_id = imu_frame_id_;
    message.orientation_covariance[0] = -1.0;  // No orientation estimate.
    message.angular_velocity.x = sample.gyro_x_radps;
    message.angular_velocity.y = sample.gyro_y_radps;
    message.angular_velocity.z = sample.gyro_z_radps;
    message.linear_acceleration.x = sample.accel_x_mps2;
    message.linear_acceleration.y = sample.accel_y_mps2;
    message.linear_acceleration.z = sample.accel_z_mps2;
    imu_publisher_->publish(message);
  }

  void publish_frame(const orbvi_sdk::FrameView& view) {
    if (view.camera_index >= static_cast<std::size_t>(camera_count_)) {
      RCLCPP_ERROR_THROTTLE(
          get_logger(), *get_clock(), 1000,
          "dropping frame with invalid camera_index=%u for configured mode",
          view.camera_index);
      return;
    }
    if (!view.valid()) {
      RCLCPP_WARN_THROTTLE(get_logger(), *get_clock(), 1000,
                           "dropping invalid FrameView for camera_%u",
                           view.camera_index);
      return;
    }
    if (view.pixel_format != orbvi_sdk::PixelFormat::gray8 ||
        view.layout != orbvi_sdk::FrameLayout::single_plane) {
      RCLCPP_WARN_THROTTLE(
          get_logger(), *get_clock(), 1000,
          "dropping unsupported camera_%u format/layout: %s/%s",
          view.camera_index, orbvi_sdk::to_string(view.pixel_format),
          orbvi_sdk::to_string(view.layout));
      return;
    }
    if (!view.crc_valid) {
      RCLCPP_WARN_THROTTLE(get_logger(), *get_clock(), 1000,
                           "dropping camera_%u frame with crc_valid=false",
                           view.camera_index);
      return;
    }
    if (view.stride != view.width) {
      RCLCPP_WARN_THROTTLE(
          get_logger(), *get_clock(), 1000,
          "dropping camera_%u frame with stride=%u width=%u; only packed mono8 "
          "frames are supported by this bridge",
          view.camera_index, view.stride, view.width);
      return;
    }

    const std::size_t width = static_cast<std::size_t>(view.width);
    const std::size_t height = static_cast<std::size_t>(view.height);
    if (height != 0U && width > std::numeric_limits<std::size_t>::max() / height) {
      RCLCPP_ERROR_THROTTLE(get_logger(), *get_clock(), 1000,
                            "dropping oversized camera_%u frame", view.camera_index);
      return;
    }
    const std::size_t expected_size = width * height;
    if (view.size < expected_size) {
      RCLCPP_WARN_THROTTLE(
          get_logger(), *get_clock(), 1000,
          "dropping truncated camera_%u frame: got=%zu expected=%zu",
          view.camera_index, view.size, expected_size);
      return;
    }

    auto message = std::make_unique<sensor_msgs::msg::Image>();
    // RTSP PTS and Fake timestamps do not establish a Unix/sensor clock.
    // Stamp on receipt using the host ROS clock; no hard-sync claim.
    message->header.stamp = now();
    message->header.frame_id = frame_id_prefix_ + std::to_string(view.camera_index);
    message->height = view.height;
    message->width = view.width;
    message->encoding = "mono8";
    message->is_bigendian = 0U;
    message->step = view.width;
    message->data.assign(view.data, view.data + expected_size);
    publishers_[view.camera_index]->publish(std::move(message));
  }

  std::string runtime_mode_;
  std::array<std::string, kCameraCount> rtsp_urls_{};
  bool imu_enabled_{false};
  std::string imu_bind_address_;
  std::string imu_source_address_;
  std::int64_t imu_port_{0};
  std::string imu_frame_id_;
  std::int64_t camera_count_{0};
  std::int64_t frames_per_camera_{0};
  std::int64_t emit_interval_us_{0};
  std::int64_t frame_width_{0};
  std::int64_t frame_height_{0};
  std::int64_t frame_stride_{0};
  std::int64_t callback_queue_capacity_{0};
  std::int64_t subscription_queue_capacity_{0};
  std::int64_t publish_queue_depth_{0};
  std::string topic_namespace_;
  std::string frame_id_prefix_;
  std::array<rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr, kCameraCount>
      publishers_{};
  rclcpp::Publisher<sensor_msgs::msg::Imu>::SharedPtr imu_publisher_;
  orbvi_sdk::Client client_;
  orbvi_sdk::SubscriptionHandle subscription_;
  orbvi_sdk::Client imu_client_;
  orbvi_sdk::SubscriptionHandle imu_subscription_;
  std::atomic<bool> shutdown_requested_{false};
};

}  // namespace

int main(int argc, char** argv) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<BridgeNode>();

  std::string error;
  if (!node->initialize(error)) {
    RCLCPP_FATAL(node->get_logger(), "%s", error.c_str());
    rclcpp::shutdown();
    return 1;
  }

  rclcpp::spin(node);
  node->shutdown();
  rclcpp::shutdown();
  return 0;
}