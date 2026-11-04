#pragma once

#include <cstddef>
#include <memory>
#include <string>

#include "orbvi_sdk/device.hpp"
#include "orbvi_sdk/runtime.hpp"
#include "orbvi_sdk/transport.hpp"

namespace orbvi_sdk {

enum class RuntimeMode {
  unsupported,
  fake,
  replay,
  rtsp,
  udp_imu,
  image_stream,
  temperature_stream,
};

enum class DropPolicy {
  drop_oldest = 0,
  drop_newest,
  block,
  // Valid only for SubscribeOptions: inherit ClientOptions.default_drop_policy.
  inherit,
};

struct ClientOptions {
  RuntimeMode runtime_mode{RuntimeMode::unsupported};
  TransportMode transport_mode{TransportMode::automatic};
  DeviceSelector selector{};
  std::string replay_path;
  std::size_t callback_queue_capacity{64U};
  DropPolicy default_drop_policy{DropPolicy::drop_oldest};
  bool require_frame_sync{true};
  std::shared_ptr<const RuntimeConfig> runtime_config;
};

struct SubscribeOptions {
  std::size_t queue_capacity{0U};
  DropPolicy drop_policy{DropPolicy::inherit};
};

}  // namespace orbvi_sdk
