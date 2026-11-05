#include <array>
#include <atomic>
#include <chrono>
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
#include "orbvi_4p_bridge/msg/frame_metadata.hpp"
#include "orbvi_4p_bridge/msg/imu_metadata.hpp"
#include "capture_helpers.hpp"
#include "frame_image.hpp"

namespace {

constexpr std::size_t kCameraCount = 4U;

class BridgeNode final : public rclcpp::Node {
 public:
  BridgeNode() : Node("orbvi_4p_bridge") {
    runtime_mode_ = declare_parameter<std::string>("runtime_mode", "unsupported");
    rtsp_urls_[0] = declare_parameter<std::string>("rtsp_url", "");
    for (std::size_t i = 1; i < kCameraCount; ++i)
      rtsp_urls_[i] = declare_parameter<std::string>("rtsp_url_" + std::to_string(i), "");
    image_format_ = declare_parameter<std::string>("image_format", "mono8");
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
    imu_subscription_queue_capacity_ =
        declare_parameter<std::int64_t>("imu_subscription_queue_capacity", 0);
    publish_queue_depth_ = declare_parameter<std::int64_t>("publish_queue_depth", 5);
    imu_publish_queue_depth_ = declare_parameter<std::int64_t>("imu_publish_queue_depth", 0);
    publish_reliability_ = declare_parameter<std::string>("publish_reliability", "best_effort");
    timestamp_source_ = declare_parameter<std::string>("timestamp_source", "host");
    publish_capture_metadata_ = declare_parameter<bool>("publish_capture_metadata", false);
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
    if (image_format_ != "mono8" &&
        !(runtime_mode_ == "rtsp" && image_format_ == "bgr8")) {
      error = "image_format must be mono8, or bgr8 for rtsp mode";
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
        imu_subscription_queue_capacity_ < 0 ||
        publish_queue_depth_ <= 0 || imu_publish_queue_depth_ < 0) {
      error = "queue capacities must be positive; IMU-specific capacities may be zero";
      return false;
    }
    if (publish_reliability_ != "best_effort" && publish_reliability_ != "reliable") {
      error = "publish_reliability must be best_effort or reliable";
      return false;
    }
    if (timestamp_source_ != "host" && timestamp_source_ != "board") {
      error = "timestamp_source must be host or board";
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
            topic_namespace_ + "/imu", sensor_qos(imu_publish_queue_depth_));
        if (publish_capture_metadata_) {
          imu_metadata_publisher_ = create_publisher<orbvi_4p_bridge::msg::ImuMetadata>(
              topic_namespace_ + "/imu/metadata", sensor_qos(imu_publish_queue_depth_));
        }
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
                static_cast<std::size_t>(imu_subscription_queue_capacity_ == 0
                                             ? subscription_queue_capacity_
                                             : imu_subscription_queue_capacity_),
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
        start_diagnostics();
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
        const auto format = image_format_ == "bgr8"
                                ? orbvi_sdk::PixelFormat::bgr8
                                : orbvi_sdk::PixelFormat::gray8;
        client_ = orbvi_sdk::make_rtsp_client(config, format);
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
      RCLCPP_INFO(get_logger(), "runtime_mode=rtsp: publishing %s camera_0..camera_%lld on %s/camera_<index>/image",
                  image_format_.c_str(), static_cast<long long>(camera_count_ - 1),
                  topic_namespace_.c_str());
    }
    if (imu_enabled_) {
      RCLCPP_INFO(get_logger(), "UDP IMU -> %s/imu", topic_namespace_.c_str());
    }
    start_diagnostics();
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
  void start_diagnostics() {
    diagnostics_timer_ = create_wall_timer(std::chrono::seconds(5), [this]() {
      const auto image_drops = subscription_.valid() ? subscription_.stats().dropped : 0U;
      const auto imu_drops = imu_subscription_.valid() ? imu_subscription_.stats().dropped : 0U;
      const auto image_failures = subscription_.valid() ? subscription_.stats().failed : 0U;
      const auto imu_failures = imu_subscription_.valid() ? imu_subscription_.stats().failed : 0U;
      const auto imu_gaps = imu_enabled_ ? imu_client_.diagnostics().imu_sequence_gaps : 0U;
      if (image_drops != reported_image_drops_ || imu_drops != reported_imu_drops_ ||
          imu_gaps != reported_imu_gaps_ || image_failures != reported_image_failures_ ||
          imu_failures != reported_imu_failures_) {
        RCLCPP_WARN(get_logger(),
                    "SDK loss counters: image_callback_drops=%llu imu_callback_drops=%llu "
                    "imu_input_sequence_gaps=%llu image_callback_failures=%llu "
                    "imu_callback_failures=%llu",
                    static_cast<unsigned long long>(image_drops),
                    static_cast<unsigned long long>(imu_drops),
                    static_cast<unsigned long long>(imu_gaps),
                    static_cast<unsigned long long>(image_failures),
                    static_cast<unsigned long long>(imu_failures));
        if (image_failures != reported_image_failures_) {
          RCLCPP_ERROR(get_logger(), "Image callback error: %s",
                       client_.diagnostics().last_error.c_str());
        }
        if (imu_failures != reported_imu_failures_) {
          RCLCPP_ERROR(get_logger(), "IMU callback error: %s",
                       imu_client_.diagnostics().last_error.c_str());
        }
        reported_image_drops_ = image_drops;
        reported_imu_drops_ = imu_drops;
        reported_imu_gaps_ = imu_gaps;
        reported_image_failures_ = image_failures;
        reported_imu_failures_ = imu_failures;
      }
    });
  }

  rclcpp::QoS sensor_qos(std::int64_t depth = 0) const {
    const auto capacity = depth == 0 ? publish_queue_depth_ : depth;
    auto qos = rclcpp::QoS(rclcpp::KeepLast(static_cast<std::size_t>(capacity)));
    if (publish_reliability_ == "reliable") qos.reliable();
    else qos.best_effort();
    return qos.durability_volatile();
  }

  void create_publishers() {
    const rclcpp::QoS qos = sensor_qos();
    for (std::size_t index = 0U; index < static_cast<std::size_t>(camera_count_); ++index) {
      const std::string topic =
          topic_namespace_ + "/camera_" + std::to_string(index) + "/image";
      publishers_[index] = create_publisher<sensor_msgs::msg::Image>(topic, qos);
      if (publish_capture_metadata_) {
        metadata_publishers_[index] = create_publisher<orbvi_4p_bridge::msg::FrameMetadata>(
            topic_namespace_ + "/camera_" + std::to_string(index) + "/metadata", qos);
      }
    }
  }

  void publish_imu(const orbvi_sdk::ImuEvent& event) {
    const auto& sample = event.sample;
    builtin_interfaces::msg::Time stamp;
    const auto timestamp = timestamp_source_ == "board" ? sample.sensor_timestamp_ns
                                                           : sample.timestamp_ns;
    if (!sample.crc_valid || !orbvi_bridge::timestamp_from_ns(timestamp, stamp) ||
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
    message.header.stamp = stamp;
    message.header.frame_id = imu_frame_id_;
    message.orientation_covariance[0] = -1.0;  // No orientation estimate.
    message.angular_velocity.x = sample.gyro_x_radps;
    message.angular_velocity.y = sample.gyro_y_radps;
    message.angular_velocity.z = sample.gyro_z_radps;
    message.linear_acceleration.x = sample.accel_x_mps2;
    message.linear_acceleration.y = sample.accel_y_mps2;
    message.linear_acceleration.z = sample.accel_z_mps2;
    imu_publisher_->publish(message);
    if (imu_metadata_publisher_) {
      orbvi_4p_bridge::msg::ImuMetadata metadata;
      metadata.header = message.header;
      metadata.sequence = sample.sequence;
      metadata.accel_timestamp_ns = sample.accel_timestamp_ns;
      metadata.gyro_timestamp_ns = sample.gyro_timestamp_ns;
      imu_metadata_publisher_->publish(metadata);
    }
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
    if (!view.crc_valid) {
      RCLCPP_WARN_THROTTLE(get_logger(), *get_clock(), 1000,
                           "dropping camera_%u frame with crc_valid=false",
                           view.camera_index);
      return;
    }
    builtin_interfaces::msg::Time capture_stamp;
    if (timestamp_source_ == "board" &&
        !orbvi_bridge::timestamp_from_ns(view.sensor_timestamp_ns, capture_stamp)) {
      RCLCPP_WARN_THROTTLE(get_logger(), *get_clock(), 1000,
                           "dropping camera_%u frame without a valid capture timestamp",
                           view.camera_index);
      return;
    }
    auto message = std::make_unique<sensor_msgs::msg::Image>();
    std::string error;
    if (!orbvi_bridge::frame_to_image(view, *message, error)) {
      RCLCPP_WARN_THROTTLE(get_logger(), *get_clock(), 1000,
                           "dropping camera_%u frame: %s",
                           view.camera_index, error.c_str());
      return;
    }
    if (timestamp_source_ == "board") {
      message->header.stamp = capture_stamp;
    } else {
      message->header.stamp = now();
    }
    message->header.frame_id = frame_id_prefix_ + std::to_string(view.camera_index);
    if (metadata_publishers_[view.camera_index] && view.sensor_timestamp_ns > 0) {
      orbvi_4p_bridge::msg::FrameMetadata metadata;
      metadata.header = message->header;
      metadata.sequence = view.sequence;
      metadata.sensor_timestamp_ns = view.sensor_timestamp_ns;
      metadata_publishers_[view.camera_index]->publish(metadata);
    }
    publishers_[view.camera_index]->publish(std::move(message));
  }

  std::string runtime_mode_;
  std::array<std::string, kCameraCount> rtsp_urls_{};
  std::string image_format_;
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
  std::int64_t imu_subscription_queue_capacity_{0};
  std::int64_t publish_queue_depth_{0};
  std::int64_t imu_publish_queue_depth_{0};
  std::string publish_reliability_;
  std::string timestamp_source_;
  bool publish_capture_metadata_{false};
  std::string topic_namespace_;
  std::string frame_id_prefix_;
  std::array<rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr, kCameraCount>
      publishers_{};
  std::array<rclcpp::Publisher<orbvi_4p_bridge::msg::FrameMetadata>::SharedPtr, kCameraCount>
      metadata_publishers_{};
  rclcpp::Publisher<sensor_msgs::msg::Imu>::SharedPtr imu_publisher_;
  rclcpp::Publisher<orbvi_4p_bridge::msg::ImuMetadata>::SharedPtr imu_metadata_publisher_;
  rclcpp::TimerBase::SharedPtr diagnostics_timer_;
  std::uint64_t reported_image_drops_{0U};
  std::uint64_t reported_imu_drops_{0U};
  std::uint64_t reported_imu_gaps_{0U};
  std::uint64_t reported_image_failures_{0U};
  std::uint64_t reported_imu_failures_{0U};
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
