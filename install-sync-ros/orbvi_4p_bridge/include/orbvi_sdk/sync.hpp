#pragma once

#include <cstdint>

#include "orbvi_sdk/contract.hpp"

namespace orbvi_sdk {

struct FrameSync {
  std::uint64_t sequence{0U};
  std::uint64_t group_id{0U};
  std::int64_t host_timestamp_ns{0};
  std::int64_t sensor_timestamp_ns{0};
  std::uint32_t generation{0U};
  std::uint32_t mask{0U};
  ContractFingerprint contract{};
  bool crc_valid{true};

  [[nodiscard]] bool complete() const noexcept {
    return (mask & 0x0fU) == 0x0fU;
  }
};

struct TimingHealth {
  std::uint64_t sample_count{0U};
  std::uint64_t missing_count{0U};
  std::int64_t last_offset_ns{0};
  std::int64_t max_abs_offset_ns{0};
  std::int32_t drift_ppb{0};
  bool degraded{false};
};

}  // namespace orbvi_sdk