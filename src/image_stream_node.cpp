// Standalone finite OIMG bridge. Never starts RTSP, Fake, or the IMU runtime.
#include <algorithm>
#include <atomic>
#include <array>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <limits>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/camera_info.hpp>
#include <sensor_msgs/msg/compressed_image.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <yaml-cpp/yaml.h>

#include "orbvi_sdk/client.hpp"
#include "orbvi_sdk/image_stream.hpp"

namespace {
constexpr std::size_t kMaxCameras = 4U;
constexpr auto kTickPeriod = std::chrono::milliseconds(100);

// Standard ROS camera calibration YAML (not a bridge-generated calibration).
bool load_calibration(const std::string& path,
                      sensor_msgs::msg::CameraInfo& info,
                      std::string& error) {
  try {
    const YAML::Node doc = YAML::LoadFile(path);
    const auto matrix = [&doc](const char* key, std::size_t rows,
                               std::size_t columns) {
      const auto field = doc[key];
      if (!field || field["rows"].as<std::size_t>() != rows ||
          field["cols"].as<std::size_t>() != columns ||
          !field["data"].IsSequence() ||
          field["data"].size() != rows * columns) {
        throw std::runtime_error(std::string("invalid calibration field: ") +
                                 key);
      }
      auto values = field["data"].as<std::vector<double>>();
      if (!std::all_of(values.begin(), values.end(),
                       [](double value) { return std::isfinite(value); })) {
        throw std::runtime_error(std::string("non-finite calibration field: ") +
                                 key);
      }
      return values;
    };

    info.width = doc["image_width"].as<std::uint32_t>();
    info.height = doc["image_height"].as<std::uint32_t>();
    info.distortion_model = doc["distortion_model"].as<std::string>();
    if (!info.width || !info.height || info.distortion_model.empty()) {
      throw std::runtime_error("calibration dimensions/distortion_model missing");
    }

    const auto distortion = doc["distortion_coefficients"];
    if (!distortion || distortion["rows"].as<std::size_t>() != 1U ||
        !distortion["data"].IsSequence() ||
        distortion["cols"].as<std::size_t>() != distortion["data"].size()) {
      throw std::runtime_error("invalid distortion_coefficients");
    }
    info.d = distortion["data"].as<std::vector<double>>();
    if (!std::all_of(info.d.begin(), info.d.end(),
                     [](double value) { return std::isfinite(value); })) {
      throw std::runtime_error("non-finite distortion_coefficients");
    }

    const auto k = matrix("camera_matrix", 3U, 3U);
    const auto r = matrix("rectification_matrix", 3U, 3U);
    const auto p = matrix("projection_matrix", 3U, 4U);
    std::copy(k.begin(), k.end(), info.k.begin());
    std::copy(r.begin(), r.end(), info.r.begin());
    std::copy(p.begin(), p.end(), info.p.begin());
    if (info.k[0] <= 0.0 || info.k[4] <= 0.0) {
      throw std::runtime_error("camera_matrix has invalid focal lengths");
    }
    return true;
  } catch (const std::exception& exception) {
    error = path + ": " + exception.what();
    return false;
  }
}

struct CameraSession {
  enum class State {
    pending,
    active,
    terminal,
  };

  std::mutex mutex;
  State state{State::pending};
  std::string terminal_reason;
  std::unique_ptr<orbvi_sdk::Client> client;
  orbvi_sdk::SubscriptionHandle subscription;
  std::uint64_t accepted{0U};
  std::uint64_t published{0U};
  std::chrono::steady_clock::time_point pending_since{};
  std::chrono::steady_clock::time_point last_frame_at{};
};

enum class TerminalKind {
  finished,
  cancelled,
  failed,
};

class ImageStreamNode final : public rclcpp::Node {
 public:
  ImageStreamNode() : Node("orbvi_image_stream") {
    mode_ = declare_parameter<std::string>("runtime_mode", "unsupported");
    host_ = declare_parameter<std::string>("host", "");
    base_port_ = declare_parameter<std::int64_t>("base_port", 5610);
    camera_mask_ = declare_parameter<std::int64_t>("camera_mask", 1);
    frames_per_camera_ =
        declare_parameter<std::int64_t>("frames_per_camera", 10);
    idle_timeout_sec_ = declare_parameter<double>("idle_timeout_sec", 30.0);
    drain_min_sec_ = declare_parameter<double>("drain_min_sec", 3.0);
    drain_timeout_sec_ = declare_parameter<double>("drain_timeout_sec", 12.0);
    publish_queue_depth_ =
        declare_parameter<std::int64_t>("publish_queue_depth", 16);
    connect_timeout_ms_ =
        declare_parameter<std::int64_t>("connect_timeout_ms", 10000);
    read_timeout_ms_ =
        declare_parameter<std::int64_t>("read_timeout_ms", 3000);
    topic_namespace_ =
        declare_parameter<std::string>("topic_namespace", "/orbvi/raw");
    frame_id_prefix_ =
        declare_parameter<std::string>("frame_id_prefix", "orbvi_camera_");
    for (std::size_t index = 0U; index < kMaxCameras; ++index) {
      calibration_paths_[index] = declare_parameter<std::string>(
          "calibration_" + std::to_string(index), "");
    }
  }

  int exit_code() const { return failed_.load() ? 1 : 0; }

  ~ImageStreamNode() override {
    for (std::size_t index = 0U; index < kMaxCameras; ++index) {
      if (is_selected(index)) {
        finalize_camera(index, TerminalKind::cancelled, "node shutdown");
      }
    }
  }

  bool initialize(std::string& error) {
    if (mode_ != "nv12" && mode_ != "jpeg") {
      error = "runtime_mode must be nv12 or jpeg";
      return false;
    }
    if (host_.empty() || base_port_ < 1024 || base_port_ > 65532 ||
        camera_mask_ < 1 || camera_mask_ > 15 || frames_per_camera_ < 1 ||
        frames_per_camera_ > 100000 || !std::isfinite(idle_timeout_sec_) ||
        idle_timeout_sec_ <= 0.0 || !std::isfinite(drain_timeout_sec_) ||
        !std::isfinite(drain_min_sec_) || drain_min_sec_ < 0.1 ||
        drain_min_sec_ > 60.0 || drain_timeout_sec_ < drain_min_sec_ ||
        drain_timeout_sec_ > 120.0 ||
        publish_queue_depth_ < 1 || publish_queue_depth_ > 256 ||
        connect_timeout_ms_ < 1 ||
        connect_timeout_ms_ > 600000 || read_timeout_ms_ < 1 ||
        read_timeout_ms_ > 600000 || topic_namespace_.empty() ||
        frame_id_prefix_.empty()) {
      error =
          "requires IPv4 host, base_port 1024..65532, camera_mask 1..15, "
          "N 1..100000, finite positive idle timeout, drain_timeout_sec "
          ">= drain_min_sec (0.1..60) and <=120, publish_queue_depth "
          "1..256, timeout_ms 1..600000, "
          "and nonempty topic/frame IDs";
      return false;
    }

    while (topic_namespace_.size() > 1U && topic_namespace_.back() == '/') {
      topic_namespace_.pop_back();
    }

    const auto initialized_at = std::chrono::steady_clock::now();
    for (std::size_t index = 0U; index < kMaxCameras; ++index) {
      sessions_[index].pending_since = initialized_at;
      if (!is_selected(index)) {
        if (!calibration_paths_[index].empty()) {
          error = "calibration configured for an unselected camera";
          return false;
        }
        continue;
      }

      if (!calibration_paths_[index].empty()) {
        if (!load_calibration(calibration_paths_[index], calibration_[index],
                              error)) {
          return false;
        }
        calibration_loaded_[index] = true;
      }

      const std::string camera_topic = camera_topic_root(index);
      // A finite burst of multi-megabyte frames must not use the shallow
      // best-effort SensorDataQoS writer. A RELIABLE writer still matches
      // existing best-effort readers; only RELIABLE readers provide ACKs.
      const auto qos = rclcpp::QoS(
          rclcpp::KeepLast(static_cast<std::size_t>(publish_queue_depth_)))
          .reliable().durability_volatile();
      if (mode_ == "nv12") {
        images_[index] =
            create_publisher<sensor_msgs::msg::Image>(camera_topic + "/image",
                                                      qos);
      } else {
        compressed_[index] =
            create_publisher<sensor_msgs::msg::CompressedImage>(
                camera_topic + "/image/compressed", qos);
      }
      camera_info_[index] =
          create_publisher<sensor_msgs::msg::CameraInfo>(
              camera_topic + "/camera_info", qos);
    }

    timer_ = create_wall_timer(kTickPeriod, [this]() { tick(); });
    RCLCPP_INFO(get_logger(),
                "finite %s sessions armed; one START per selected camera, no "
                "RTSP/Fake/IMU",
                mode_.c_str());
    return true;
  }

 private:
  bool is_selected(std::size_t index) const {
    return index < kMaxCameras && (camera_mask_ & (1 << index)) != 0;
  }

  std::string camera_topic_root(std::size_t index) const {
    const std::string prefix = topic_namespace_ == "/" ? std::string{} : topic_namespace_;
    return prefix + "/camera_" + std::to_string(index);
  }

  std::size_t subscription_count(std::size_t index) const {
    if (mode_ == "nv12") {
      return images_[index]->get_subscription_count();
    }
    return compressed_[index]->get_subscription_count();
  }

  // Snapshot matched reliable readers at the start of the drain. Best-effort
  // readers cannot ACK; graph discovery is not an application-level receipt.
  bool has_reliable_reader(const std::string& topic) const {
    const auto endpoints = get_subscriptions_info_by_topic(topic);
    return std::any_of(endpoints.begin(), endpoints.end(), [](const auto& endpoint) {
      return endpoint.qos_profile().reliability() ==
             rclcpp::ReliabilityPolicy::Reliable;
    });
  }

  void tick() {
    for (std::size_t index = 0U; index < kMaxCameras; ++index) {
      if (is_selected(index)) {
        tick_camera(index);
      }
    }
    if (!all_selected_terminal()) {
      return;
    }
    const auto now_steady = std::chrono::steady_clock::now();
    if (drain_started_at_ == std::chrono::steady_clock::time_point{}) {
      drain_started_at_ = now_steady;
      try {
        for (std::size_t index = 0; index < kMaxCameras; ++index) {
          if (!is_selected(index)) {
            continue;
          }
          const auto image_topic = mode_ == "nv12"
              ? images_[index]->get_topic_name()
              : compressed_[index]->get_topic_name();
          reliable_image_[index] = has_reliable_reader(image_topic);
          reliable_info_[index] =
              has_reliable_reader(camera_info_[index]->get_topic_name());
        }
      } catch (const std::exception& exception) {
        failed_.store(true);
        RCLCPP_ERROR(get_logger(), "cannot inspect ROS reader QoS: %s", exception.what());
      }
      RCLCPP_INFO(get_logger(),
                  "all camera sessions terminal; retaining ROS publishers "
                  "for at least %.2fs (ACK deadline %.2fs)",
                  drain_min_sec_, drain_timeout_sec_);
      return;
    }
    const double elapsed =
        std::chrono::duration<double>(now_steady - drain_started_at_).count();
    if (elapsed < drain_min_sec_ || shutdown_requested_.load()) {
      return;
    }

    bool all_reliable_acked = true;
    try {
      for (std::size_t index = 0; index < kMaxCameras; ++index) {
        if (!is_selected(index)) {
          continue;
        }
        // RMW ACK covers a reliable DDS reader's receive queue, not its
        // application callback. Never mistake it for a best-effort receipt.
        const auto probe = std::chrono::milliseconds(50);
        const bool image_acked = !reliable_image_[index] ||
            (mode_ == "nv12" ? images_[index]->wait_for_all_acked(probe)
                             : compressed_[index]->wait_for_all_acked(probe));
        const bool info_acked = !reliable_info_[index] ||
            camera_info_[index]->wait_for_all_acked(probe);
        if (!image_acked || !info_acked) {
          all_reliable_acked = false;
          if (elapsed >= drain_timeout_sec_) {
            failed_.store(true);
            RCLCPP_ERROR(get_logger(),
                         "camera_%zu reliable ROS ACK deadline exceeded "
                         "(image=%d info=%d)", index, image_acked, info_acked);
          }
        }
      }
    } catch (const std::exception& exception) {
      failed_.store(true);
      RCLCPP_ERROR(get_logger(), "ROS drain ACK check failed: %s", exception.what());
      all_reliable_acked = false;
    }
    if (!all_reliable_acked && elapsed < drain_timeout_sec_) {
      return;
    }
    if (!shutdown_requested_.exchange(true)) {
      // Best-effort readers have no ACK. Even a reliable ACK does not prove
      // that a subscriber application has executed all its callbacks.
      RCLCPP_INFO(get_logger(), "ROS drain finished (%.2fs); finite N counts "
                  "local publishes, not best-effort subscriber deliveries", elapsed);
      rclcpp::shutdown();
    }
  }

  void tick_camera(std::size_t index) {
    bool should_start = false;
    bool complete = false;
    bool subscriber_gone = false;
    bool idle_expired = false;
    bool active_idle_expired = false;
    {
      const std::lock_guard<std::mutex> lock(sessions_[index].mutex);
      CameraSession& session = sessions_[index];
      if (session.state == CameraSession::State::terminal) {
        return;
      }

      if (session.state == CameraSession::State::pending) {
        if (mode_ == "jpeg" && subscription_count(index) == 0U) {
          idle_expired =
              std::chrono::duration<double>(std::chrono::steady_clock::now() -
                                            session.pending_since)
                  .count() >= idle_timeout_sec_;
        } else {
          should_start = true;
        }
      } else {
        if (session.published >=
            static_cast<std::uint64_t>(frames_per_camera_)) {
          complete = true;
        } else if (mode_ == "jpeg" && subscription_count(index) == 0U) {
          subscriber_gone = true;
        } else if (std::chrono::duration<double>(
                       std::chrono::steady_clock::now() - session.last_frame_at)
                       .count() >= idle_timeout_sec_) {
          active_idle_expired = true;
        }
      }
    }

    if (idle_expired) {
      finalize_camera(index, TerminalKind::failed,
                      "JPEG subscriber idle timeout before START; requested N not delivered");
      return;
    }
    if (active_idle_expired) {
      finalize_camera(index, TerminalKind::failed,
                      "idle timeout before requested N; no reconnect");
      return;
    }
    if (complete) {
      finalize_camera(index, TerminalKind::finished, "requested N delivered");
      return;
    }
    if (subscriber_gone) {
      finalize_camera(index, TerminalKind::failed,
                      "JPEG subscriber disappeared before N; no restart");
      return;
    }
    if (should_start) {
      start_camera(index);
      return;
    }
    // The SDK TCP reader can stop after receiving the Nth wire frame while
    // its subscription worker still has valid frames queued for ROS delivery.
    // Do not cancel that queue after an arbitrary grace period: the ordinary
    // idle deadline bounds both a stalled reader and a draining callback.
  }

  void start_camera(std::size_t index) {
    orbvi_sdk::ImageStreamConfig config;
    config.host = host_;
    config.base_port = static_cast<std::uint16_t>(base_port_);
    config.camera_mask = static_cast<std::uint8_t>(1U << index);
    config.frames_per_camera =
        static_cast<std::uint32_t>(frames_per_camera_);
    config.format = mode_ == "nv12" ? orbvi_sdk::ImageStreamFormat::nv12
                                    : orbvi_sdk::ImageStreamFormat::jpeg;
    config.connect_timeout =
        std::chrono::milliseconds(connect_timeout_ms_);
    config.read_timeout = std::chrono::milliseconds(read_timeout_ms_);

    auto client = std::make_unique<orbvi_sdk::Client>(
        orbvi_sdk::make_image_stream_client(config));
    const auto opened = client->open();
    if (!opened.ok()) {
      finalize_camera(index, TerminalKind::failed,
                      "SDK open failed: " + opened.to_string());
      return;
    }

    orbvi_sdk::CallbackSet callbacks;
    if (mode_ == "nv12") {
      callbacks.on_frame = [this, index](const orbvi_sdk::FrameView& frame) {
        publish_nv12(index, frame);
      };
    } else {
      callbacks.on_compressed_frame =
          [this, index](const orbvi_sdk::CompressedFrameView& frame) {
            publish_jpeg(index, frame);
          };
    }

    auto subscription = client->subscribe(
        orbvi_sdk::SubscribeOptions{8U, orbvi_sdk::DropPolicy::drop_oldest},
        std::move(callbacks));
    if (!subscription.ok()) {
      static_cast<void>(client->close());
      finalize_camera(index, TerminalKind::failed,
                      "SDK subscribe failed: " +
                          subscription.status().to_string());
      return;
    }

    {
      const std::lock_guard<std::mutex> lock(sessions_[index].mutex);
      CameraSession& session = sessions_[index];
      if (session.state != CameraSession::State::pending) {
        static_cast<void>(client->close());
        return;
      }
      session.client = std::move(client);
      session.subscription = std::move(subscription.value());
      session.state = CameraSession::State::active;
      session.last_frame_at = std::chrono::steady_clock::now();
    }

    const auto started = sessions_[index].client->start();
    if (!started.ok()) {
      finalize_camera(index, TerminalKind::failed,
                      "SDK START failed: " + started.to_string());
    }
  }

  void finalize_camera(std::size_t index, TerminalKind kind,
                       const std::string& reason) {
    std::unique_ptr<orbvi_sdk::Client> client;
    orbvi_sdk::SubscriptionHandle subscription;
    bool already_terminal = false;
    {
      const std::lock_guard<std::mutex> lock(sessions_[index].mutex);
      CameraSession& session = sessions_[index];
      if (session.state == CameraSession::State::terminal) {
        already_terminal = true;
      } else {
        session.state = CameraSession::State::terminal;
        session.terminal_reason = reason;
        client = std::move(session.client);
        subscription = std::move(session.subscription);
      }
    }
    if (already_terminal) {
      return;
    }
    if (kind == TerminalKind::failed) {
      failed_.store(true);
    }

    // Never hold a session mutex while joining SDK threads: an in-flight
    // callback may legitimately need the same mutex before it can exit.
    subscription.cancel();
    if (client) {
      try {
        static_cast<void>(client->stop());
      } catch (...) {
      }
      try {
        static_cast<void>(client->close());
      } catch (...) {
      }
    }

    if (kind == TerminalKind::finished) {
      RCLCPP_INFO(get_logger(), "camera_%zu terminal: %s", index,
                  reason.c_str());
    } else if (kind == TerminalKind::cancelled) {
      RCLCPP_WARN(get_logger(), "camera_%zu terminal: %s", index,
                  reason.c_str());
    } else {
      RCLCPP_ERROR(get_logger(), "camera_%zu terminal: %s", index,
                   reason.c_str());
    }
  }

  bool all_selected_terminal() {
    for (std::size_t index = 0U; index < kMaxCameras; ++index) {
      if (!is_selected(index)) {
        continue;
      }
      const std::lock_guard<std::mutex> lock(sessions_[index].mutex);
      if (sessions_[index].state != CameraSession::State::terminal) {
        return false;
      }
    }
    return true;
  }

  bool reserve_frame(std::size_t index) {
    const std::lock_guard<std::mutex> lock(sessions_[index].mutex);
    CameraSession& session = sessions_[index];
    if (session.state != CameraSession::State::active ||
        session.accepted >=
            static_cast<std::uint64_t>(frames_per_camera_)) {
      return false;
    }
    ++session.accepted;
    return true;
  }

  void frame_published(std::size_t index) {
    const std::lock_guard<std::mutex> lock(sessions_[index].mutex);
    ++sessions_[index].published;
    sessions_[index].last_frame_at = std::chrono::steady_clock::now();
  }

  void publish_info(std::size_t index, const std_msgs::msg::Header& header,
                    std::uint32_t width, std::uint32_t height) {
    auto info = calibration_[index];
    // No calibration file: ROS's uncalibrated convention is zero K/P, empty D.
    // Dimensions and the frame header are metadata, not fabricated intrinsics.
    info.width = width;
    info.height = height;
    info.header = header;
    camera_info_[index]->publish(info);
  }

  bool calibration_matches(std::size_t index, std::uint32_t width,
                           std::uint32_t height) {
    if (!calibration_loaded_[index]) {
      return true;
    }
    if (calibration_[index].width == width &&
        calibration_[index].height == height) {
      return true;
    }
    RCLCPP_ERROR_THROTTLE(get_logger(), *get_clock(), 1000,
                          "camera_%zu calibration dimensions differ from frame; "
                          "frame dropped",
                          index);
    return false;
  }

  void publish_nv12(std::size_t index, const orbvi_sdk::FrameView& frame) {
    const auto width = static_cast<std::size_t>(frame.width);
    const auto height = static_cast<std::size_t>(frame.height);
    if (index >= kMaxCameras || frame.camera_index != index || !frame.valid() ||
        !frame.crc_valid ||
        frame.pixel_format != orbvi_sdk::PixelFormat::nv12 || width == 0U ||
        height == 0U || (width & 1U) != 0U || (height & 1U) != 0U ||
        frame.stride != frame.width ||
        width > std::numeric_limits<std::size_t>::max() / height ||
        width * height >
            std::numeric_limits<std::size_t>::max() - width * height / 2U ||
        frame.size != width * height + width * height / 2U ||
        !calibration_matches(index, frame.width, frame.height) ||
        !reserve_frame(index)) {
      return;
    }

    sensor_msgs::msg::Image message;
    // Host receipt clock. The board capture clock is metadata, not ROS wall time.
    message.header.stamp = now();
    message.header.frame_id = frame_id_prefix_ + std::to_string(index);
    message.width = frame.width;
    message.height = frame.height;
    // Project-specific encoding: tightly packed Y, then interleaved UV.
    // step=width, while data intentionally contains 1.5*height rows of bytes.
    message.encoding = "nv12";
    message.step = frame.width;
    message.is_bigendian = 0U;
    message.data.assign(frame.data, frame.data + frame.size);
    publish_info(index, message.header, message.width, message.height);
    images_[index]->publish(message);
    frame_published(index);
  }

  void publish_jpeg(std::size_t index,
                    const orbvi_sdk::CompressedFrameView& frame) {
    if (index >= kMaxCameras || frame.camera_index != index || frame.data == nullptr ||
        frame.size == 0U || frame.width == 0U || frame.height == 0U ||
        !frame.crc_valid ||
        !calibration_matches(index, frame.width, frame.height) ||
        !reserve_frame(index)) {
      return;
    }

    sensor_msgs::msg::CompressedImage message;
    message.header.stamp = now();
    message.header.frame_id = frame_id_prefix_ + std::to_string(index);
    message.format = "jpeg";
    message.data.assign(frame.data, frame.data + frame.size);
    publish_info(index, message.header, frame.width, frame.height);
    compressed_[index]->publish(message);
    frame_published(index);
  }

  std::string mode_;
  std::string host_;
  std::string topic_namespace_;
  std::string frame_id_prefix_;
  std::int64_t base_port_{};
  std::int64_t camera_mask_{};
  std::int64_t frames_per_camera_{};
  double idle_timeout_sec_{};
  double drain_min_sec_{};
  double drain_timeout_sec_{};
  std::int64_t publish_queue_depth_{};
  std::int64_t connect_timeout_ms_{};
  std::int64_t read_timeout_ms_{};
  std::array<std::string, kMaxCameras> calibration_paths_{};
  std::array<sensor_msgs::msg::CameraInfo, kMaxCameras> calibration_{};
  std::array<bool, kMaxCameras> calibration_loaded_{};
  std::array<rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr,
             kMaxCameras>
      images_{};
  std::array<rclcpp::Publisher<sensor_msgs::msg::CompressedImage>::SharedPtr,
             kMaxCameras>
      compressed_{};
  std::array<rclcpp::Publisher<sensor_msgs::msg::CameraInfo>::SharedPtr,
             kMaxCameras>
      camera_info_{};
  std::array<CameraSession, kMaxCameras> sessions_{};
  rclcpp::TimerBase::SharedPtr timer_;
  std::chrono::steady_clock::time_point drain_started_at_{};
  std::array<bool, kMaxCameras> reliable_image_{};
  std::array<bool, kMaxCameras> reliable_info_{};
  std::atomic<bool> shutdown_requested_{false};
  std::atomic<bool> failed_{false};
};
}  // namespace

int main(int argc, char** argv) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<ImageStreamNode>();
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
