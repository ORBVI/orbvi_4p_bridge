#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <sensor_msgs/msg/imu.hpp>

#include "orbvi_4p_bridge/msg/frame_metadata.hpp"
#include "orbvi_4p_bridge/msg/imu_metadata.hpp"
#include "capture_statistics.hpp"

namespace {

using Clock = std::chrono::steady_clock;

std::int64_t timestamp_ns(const builtin_interfaces::msg::Time& stamp) {
  return static_cast<std::int64_t>(stamp.sec) * 1000000000LL + stamp.nanosec;
}

using Statistics = orbvi_bridge::CaptureStatistics;

class CaptureCheck final : public rclcpp::Node {
 public:
  CaptureCheck(int warmup_seconds, int duration_seconds)
      : Node("orbvi_capture_check"),
        start_(Clock::now() + std::chrono::seconds(warmup_seconds)),
        end_(start_ + std::chrono::seconds(duration_seconds + 5)),
        duration_(duration_seconds),
        duration_ns_(static_cast<std::int64_t>(duration_seconds) * 1000000000LL) {
    const auto qos = rclcpp::QoS(rclcpp::KeepLast(32)).reliable().durability_volatile();
    const auto metadata_qos = rclcpp::QoS(rclcpp::KeepLast(256)).reliable().durability_volatile();
    for (std::size_t index = 0; index < cameras_.size(); ++index) {
      auto group = create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);
      groups_.push_back(group);
      rclcpp::SubscriptionOptions options;
      auto image_group = create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive, false);
      image_groups_.push_back(image_group);
      options.callback_group = image_group;
      const auto base = "/orbvi/raw/camera_" + std::to_string(index);
      image_subscriptions_[index] = create_subscription<sensor_msgs::msg::Image>(
          base + "/image", qos,
          [this, index](sensor_msgs::msg::Image::ConstSharedPtr message) {
            const auto stamp = timestamp_ns(message->header.stamp);
            const bool valid = stamp > 0 && message->encoding == "bgr8" &&
                message->width > 0U && message->height > 0U &&
                message->step == message->width * 3U &&
                message->data.size() == static_cast<std::size_t>(message->step) * message->height &&
                message->header.frame_id == "orbvi_camera_" + std::to_string(index);
            cameras_[index].add_data(stamp, valid, Clock::now() >= start_);
          }, options);
      options.callback_group = group;
      frame_metadata_subscriptions_[index] =
          create_subscription<orbvi_4p_bridge::msg::FrameMetadata>(
              base + "/metadata", metadata_qos,
              [this, index](orbvi_4p_bridge::msg::FrameMetadata::ConstSharedPtr message) {
                const auto stamp = timestamp_ns(message->header.stamp);
                const bool valid = stamp > 0 && stamp == message->sensor_timestamp_ns &&
                    message->header.frame_id == "orbvi_camera_" + std::to_string(index);
                cameras_[index].add_metadata(stamp, message->sequence, valid,
                                             Clock::now() >= start_);
              }, options);
    }
    auto group = create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);
    groups_.push_back(group);
    rclcpp::SubscriptionOptions options;
    options.callback_group = group;
    const auto imu_qos = rclcpp::QoS(rclcpp::KeepLast(2048)).reliable().durability_volatile();
    imu_subscription_ = create_subscription<sensor_msgs::msg::Imu>(
        "/orbvi/raw/imu", imu_qos, [this](sensor_msgs::msg::Imu::ConstSharedPtr message) {
          const auto stamp = timestamp_ns(message->header.stamp);
          const bool valid = stamp > 0 && message->header.frame_id == "orbvi_imu" &&
              std::isfinite(message->linear_acceleration.x) &&
              std::isfinite(message->linear_acceleration.y) &&
              std::isfinite(message->linear_acceleration.z) &&
              std::isfinite(message->angular_velocity.x) &&
              std::isfinite(message->angular_velocity.y) &&
              std::isfinite(message->angular_velocity.z);
          imu_.add_data(stamp, valid, Clock::now() >= start_);
        }, options);
    imu_metadata_subscription_ = create_subscription<orbvi_4p_bridge::msg::ImuMetadata>(
        "/orbvi/raw/imu/metadata", imu_qos,
        [this](orbvi_4p_bridge::msg::ImuMetadata::ConstSharedPtr message) {
          const auto stamp = timestamp_ns(message->header.stamp);
          const bool valid = stamp > 0 &&
              stamp == std::min(message->accel_timestamp_ns, message->gyro_timestamp_ns) &&
              message->header.frame_id == "orbvi_imu";
          imu_.add_metadata(stamp, message->sequence, valid, Clock::now() >= start_);
        }, options);
  }

  Clock::time_point end() const { return end_; }

  const std::vector<rclcpp::CallbackGroup::SharedPtr>& image_groups() const {
    return image_groups_;
  }

  bool complete() const {
    return std::all_of(cameras_.begin(), cameras_.end(),
                       [this](const auto& stats) { return stats.complete(duration_ns_, true); }) &&
        imu_.complete(duration_ns_);
  }

  bool report() const {
    bool valid = complete();
    for (std::size_t index = 0; index < cameras_.size(); ++index) {
      const auto stats = cameras_[index].summarize(duration_ns_, true);
      const auto fps = static_cast<double>(stats.count) / duration_;
      std::cout << "camera=" << index << " images=" << stats.count << " fps=" << fps
                << " metadata=" << stats.metadata_count << " sequence_gaps=" << stats.gaps
                << " discontinuities=" << stats.discontinuities
                << " image_without_metadata=" << stats.missing_metadata
                << " metadata_without_image=" << stats.missing_data
                << " invalid=" << stats.invalid << '\n';
      if (fps < 29.0 || fps > 31.0 || stats.metadata_count == 0U || stats.gaps ||
          stats.discontinuities || stats.invalid || stats.missing_metadata || stats.missing_data) valid = false;
    }
    const auto imu = imu_.summarize(duration_ns_, false);
    std::cout << "imu=" << imu.count << " rate=" << static_cast<double>(imu.count) / duration_
              << " metadata=" << imu.metadata_count << " sequence_gaps=" << imu.gaps
              << " discontinuities=" << imu.discontinuities
              << " imu_without_metadata=" << imu.missing_metadata
              << " metadata_without_imu=" << imu.missing_data
              << " invalid=" << imu.invalid << " window_complete=" << complete()
              << " window_start_ns=" << imu.window_start
              << " window_end_ns=" << imu.window_end
              << " first_missing_imu_ns=" << imu.first_missing_data
              << " first_missing_metadata_ns=" << imu.first_missing_metadata << '\n';
    if (!imu.count || !imu.metadata_count || imu.gaps || imu.discontinuities ||
        imu.invalid || imu.missing_metadata || imu.missing_data) valid = false;
    std::cout << (valid ? "PASS" : "FAIL") << '\n';
    return valid;
  }

 private:
  Clock::time_point start_;
  Clock::time_point end_;
  int duration_;
  std::int64_t duration_ns_;
  std::array<Statistics, 4U> cameras_{};
  Statistics imu_;
  std::vector<rclcpp::CallbackGroup::SharedPtr> groups_;
  std::vector<rclcpp::CallbackGroup::SharedPtr> image_groups_;
  std::array<rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr, 4U> image_subscriptions_{};
  std::array<rclcpp::Subscription<orbvi_4p_bridge::msg::FrameMetadata>::SharedPtr, 4U>
      frame_metadata_subscriptions_{};
  rclcpp::Subscription<sensor_msgs::msg::Imu>::SharedPtr imu_subscription_;
  rclcpp::Subscription<orbvi_4p_bridge::msg::ImuMetadata>::SharedPtr imu_metadata_subscription_;
};

}

int main(int argc, char** argv) {
  int duration = 60;
  if (argc > 2) {
    std::cerr << "usage: orbvi_capture_check [duration_seconds]\n";
    return 2;
  }
  if (argc == 2) {
    try {
      const std::string value(argv[1]);
      std::size_t consumed = 0U;
      duration = std::stoi(value, &consumed);
      if (consumed != value.size() || duration < 1 || duration > 3600) throw std::invalid_argument("duration");
    } catch (const std::exception&) {
      std::cerr << "duration_seconds must be an integer between 1 and 3600\n";
      return 2;
    }
  }
  rclcpp::init(argc, argv);
  auto node = std::make_shared<CaptureCheck>(5, duration);
  rclcpp::executors::SingleThreadedExecutor executor;
  rclcpp::executors::SingleThreadedExecutor image_executor;
  executor.add_node(node);
  for (const auto& group : node->image_groups()) {
    image_executor.add_callback_group(group, node->get_node_base_interface());
  }
  std::thread image_worker([&image_executor]() { image_executor.spin(); });
  try {
    while (rclcpp::ok() && Clock::now() < node->end() && !node->complete()) {
      executor.spin_once(std::chrono::milliseconds(100));
    }
  } catch (...) {
    image_executor.cancel();
    image_worker.join();
    throw;
  }
  image_executor.cancel();
  image_worker.join();
  const bool result = node->report();
  rclcpp::shutdown();
  return result ? 0 : 1;
}
