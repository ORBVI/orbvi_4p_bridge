#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "orbvi_sdk/device.hpp"
#include "orbvi_sdk/export.hpp"
#include "orbvi_sdk/runtime.hpp"

namespace orbvi_sdk {

class Client;

enum class FakeFault {
  none,
  missing_sequence,
  out_of_order,
  crc_error,
  truncated_jpeg,
  time_rollback,
  generation_change,
  disconnect_reconnect,
};

struct FakeConfig final : RuntimeConfig {
  std::vector<DeviceInfo> devices;
  std::uint64_t frames_per_camera{4U};
  std::uint64_t imu_samples{8U};
  std::uint32_t frame_width{1280U};
  std::uint32_t frame_height{1088U};
  std::uint32_t frame_stride{1280U};
  std::uint32_t emit_interval_us{1000U};
  std::uint32_t fault_after_frames{2U};
  std::uint32_t fault_camera{0U};
  FakeFault fault{FakeFault::none};
  bool emit_frame_sync{true};
  bool emit_timing_health{true};
  bool auto_emit{true};
};

ORBVI_SDK_API Client make_fake_client(const FakeConfig& config);

ORBVI_SDK_API FakeConfig make_default_fake_config(std::uint32_t device_count = 1U);

}  // namespace orbvi_sdk