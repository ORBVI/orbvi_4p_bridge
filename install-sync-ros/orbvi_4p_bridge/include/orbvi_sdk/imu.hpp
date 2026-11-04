#pragma once

#include <cstdint>

#include "orbvi_sdk/contract.hpp"
#include "orbvi_sdk/sync.hpp"

namespace orbvi_sdk {

struct ImuSample {
  std::uint64_t sequence{0U};
  std::int64_t timestamp_ns{0};
  std::int64_t sensor_timestamp_ns{0};
  std::uint32_t generation{0U};
  double accel_x_mps2{0.0};
  double accel_y_mps2{0.0};
  double accel_z_mps2{0.0};
  double gyro_x_radps{0.0};
  double gyro_y_radps{0.0};
  double gyro_z_radps{0.0};
  float temperature_c{0.0F};
  ContractFingerprint contract{};
  bool crc_valid{true};
  // Sender IIO/board clock domains, not synchronized with host or Unix time.
  std::int64_t accel_timestamp_ns{0};
  std::int64_t gyro_timestamp_ns{0};
  std::uint64_t board_send_timestamp_ns{0U};
};

struct ImuEvent {
  ImuSample sample{};
  TimingHealth timing{};
};

}  // namespace orbvi_sdk
