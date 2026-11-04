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
#include <unordered_map>
#include <vector>

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <sensor_msgs/msg/imu.hpp>

#include "orbvi_4p_bridge/msg/frame_metadata.hpp"
#include "orbvi_4p_bridge/msg/imu_metadata.hpp"

namespace {

using Clock = std::chrono::steady_clock;

std::int64_t timestamp_ns(const builtin_interfaces::msg::Time& stamp) {
  return static_cast<std::int64_t>(stamp.sec) * 1000000000LL + stamp.nanosec;
}

struct Statistics {
  std::uint64_t count{0U};
  std::uint64_t metadata_count{0U};
  std::uint64_t gaps{0U};
  std::uint64_t discontinuities{0U};
  std::uint64_t invalid{0U};
  std::uint64_t previous_sequence{0U};
  bool has_sequence{false};
  std::unordered_map<std::int64_t, std::size_t> timestamps;
  std::unordered_map<std::int64_t, std::size_t> metadata_timestamps;
};

struct Unmatched {
  std::size_t data{0U};
  std::size_t metadata{0U};
};

Unmatched unmatched(const Statistics& stats) {
  if (stats.timestamps.empty() || stats.metadata_timestamps.empty()) {
    return {stats.timestamps.size(), stats.metadata_timestamps.size()};
  }
  const auto less = [](const auto& left, const auto& right) { return left.first < right.first; };
  const auto data_bounds =
      std::minmax_element(stats.timestamps.begin(), stats.timestamps.end(), less);
  const auto metadata_bounds =
      std::minmax_element(stats.metadata_timestamps.begin(), stats.metadata_timestamps.end(), less);
  const auto first = std::max(data_bounds.first->first, metadata_bounds.first->first);
  const auto last = std::min(data_bounds.second->first, metadata_bounds.second->first);
  if (first > last) return {stats.timestamps.size(), stats.metadata_timestamps.size()};
  Unmatched result;
  for (const auto& entry : stats.timestamps) {
    if (entry.first < first || entry.first > last) continue;
    const auto found = stats.metadata_timestamps.find(entry.first);
    const auto matched = found == stats.metadata_timestamps.end() ? 0U : found->second;
    if (entry.second > matched) result.data += entry.second - matched;
  }
  for (const auto& entry : stats.metadata_timestamps) {
    if (entry.first < first || entry.first > last) continue;
    const auto found = stats.timestamps.find(entry.first);
    const auto matched = found == stats.timestamps.end() ? 0U : found->second;
    if (entry.second > matched) result.metadata += entry.second - matched;
  }
  return result;
}

class CaptureCheck final : public rclcpp::Node {
 public:
  CaptureCheck(int warmup_seconds, int duration_seconds)
      : Node("orbvi_capture_check"),
        start_(Clock::now() + std::chrono::seconds(warmup_seconds)),
        end_(start_ + std::chrono::seconds(duration_seconds)),
        duration_(duration_seconds) {
    const auto qos = rclcpp::QoS(rclcpp::KeepLast(32)).reliable().durability_volatile();
    for (std::size_t index = 0; index < cameras_.size(); ++index) {
      auto group = create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);
      groups_.push_back(group);
      rclcpp::SubscriptionOptions options;
      options.callback_group = group;
      const auto base = "/orbvi/raw/camera_" + std::to_string(index);
      image_subscriptions_[index] = create_subscription<sensor_msgs::msg::Image>(
          base + "/image", qos,
          [this, index](sensor_msgs::msg::Image::ConstSharedPtr message) {
            if (!recording()) return;
            auto& stats = cameras_[index];
            const auto stamp = timestamp_ns(message->header.stamp);
            ++stats.count;
            ++stats.timestamps[stamp];
            if (stamp <= 0 || message->encoding != "bgr8" || message->width == 0U ||
                message->height == 0U || message->step != message->width * 3U ||
                message->data.size() != static_cast<std::size_t>(message->step) * message->height) {
              ++stats.invalid;
            }
          }, options);
      frame_metadata_subscriptions_[index] =
          create_subscription<orbvi_4p_bridge::msg::FrameMetadata>(
              base + "/metadata", qos,
              [this, index](orbvi_4p_bridge::msg::FrameMetadata::ConstSharedPtr message) {
                if (!recording()) return;
                auto& stats = cameras_[index];
                const auto stamp = timestamp_ns(message->header.stamp);
                ++stats.metadata_count;
                ++stats.metadata_timestamps[stamp];
                if (stamp <= 0 || stamp != message->sensor_timestamp_ns) ++stats.invalid;
                if (stats.has_sequence) {
                  const auto delta = static_cast<std::uint32_t>(
                      message->sequence - stats.previous_sequence);
                  if (delta == 0U || delta >= 0x80000000U) ++stats.discontinuities;
                  else if (delta > 1U) stats.gaps += delta - 1U;
                }
                stats.previous_sequence = message->sequence;
                stats.has_sequence = true;
              }, options);
    }
    auto group = create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);
    groups_.push_back(group);
    rclcpp::SubscriptionOptions options;
    options.callback_group = group;
    const auto imu_qos = rclcpp::QoS(rclcpp::KeepLast(2048)).reliable().durability_volatile();
    imu_subscription_ = create_subscription<sensor_msgs::msg::Imu>(
        "/orbvi/raw/imu", imu_qos, [this](sensor_msgs::msg::Imu::ConstSharedPtr message) {
          if (!recording()) return;
          const auto stamp = timestamp_ns(message->header.stamp);
          ++imu_.count;
          ++imu_.timestamps[stamp];
          if (stamp <= 0 || !std::isfinite(message->linear_acceleration.x) ||
              !std::isfinite(message->linear_acceleration.y) ||
              !std::isfinite(message->linear_acceleration.z) ||
              !std::isfinite(message->angular_velocity.x) ||
              !std::isfinite(message->angular_velocity.y) ||
              !std::isfinite(message->angular_velocity.z)) {
            ++imu_.invalid;
          }
        }, options);
    imu_metadata_subscription_ = create_subscription<orbvi_4p_bridge::msg::ImuMetadata>(
        "/orbvi/raw/imu/metadata", imu_qos,
        [this](orbvi_4p_bridge::msg::ImuMetadata::ConstSharedPtr message) {
          if (!recording()) return;
          const auto stamp = timestamp_ns(message->header.stamp);
          ++imu_.metadata_count;
          ++imu_.metadata_timestamps[stamp];
          if (stamp <= 0 || stamp != std::min(message->accel_timestamp_ns,
                                               message->gyro_timestamp_ns)) {
            ++imu_.invalid;
          }
          if (imu_.has_sequence) {
            if (message->sequence <= imu_.previous_sequence) ++imu_.discontinuities;
            else if (message->sequence - imu_.previous_sequence > 1U) {
              imu_.gaps += message->sequence - imu_.previous_sequence - 1U;
            }
          }
          imu_.previous_sequence = message->sequence;
          imu_.has_sequence = true;
        }, options);
  }

  Clock::time_point end() const { return end_; }

  bool report() const {
    bool valid = true;
    for (std::size_t index = 0; index < cameras_.size(); ++index) {
      const auto& stats = cameras_[index];
      const auto missing = unmatched(stats);
      const auto fps = static_cast<double>(stats.count) / duration_;
      std::cout << "camera=" << index << " images=" << stats.count << " fps=" << fps
                << " metadata=" << stats.metadata_count << " sequence_gaps=" << stats.gaps
                << " discontinuities=" << stats.discontinuities
                << " image_without_metadata=" << missing.data
                << " metadata_without_image=" << missing.metadata
                << " invalid=" << stats.invalid << '\n';
      if (fps < 29.0 || fps > 31.0 || stats.metadata_count == 0U || stats.gaps ||
          stats.discontinuities || stats.invalid || missing.data || missing.metadata) valid = false;
    }
    const auto missing = unmatched(imu_);
    std::cout << "imu=" << imu_.count << " rate=" << static_cast<double>(imu_.count) / duration_
              << " metadata=" << imu_.metadata_count << " sequence_gaps=" << imu_.gaps
              << " discontinuities=" << imu_.discontinuities
              << " imu_without_metadata=" << missing.data
              << " metadata_without_imu=" << missing.metadata
              << " invalid=" << imu_.invalid << '\n';
    if (!imu_.count || !imu_.metadata_count || imu_.gaps || imu_.discontinuities ||
        imu_.invalid || missing.data || missing.metadata) valid = false;
    std::cout << (valid ? "PASS" : "FAIL") << '\n';
    return valid;
  }

 private:
  bool recording() const {
    const auto now = Clock::now();
    return now >= start_ && now < end_;
  }

  Clock::time_point start_;
  Clock::time_point end_;
  int duration_;
  std::array<Statistics, 4U> cameras_{};
  Statistics imu_;
  std::vector<rclcpp::CallbackGroup::SharedPtr> groups_;
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
  rclcpp::executors::MultiThreadedExecutor executor(rclcpp::ExecutorOptions(), 6);
  executor.add_node(node);
  std::thread worker([&executor]() { executor.spin(); });
  std::this_thread::sleep_until(node->end());
  executor.cancel();
  worker.join();
  const bool result = node->report();
  rclcpp::shutdown();
  return result ? 0 : 1;
}
