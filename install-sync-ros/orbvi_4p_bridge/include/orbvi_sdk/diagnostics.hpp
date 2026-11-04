#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>

#include "orbvi_sdk/export.hpp"

namespace orbvi_sdk {

enum class ClientState {
  closed = 0,
  open,
  started,
  stopped,
  failed,
};

struct DiagnosticsSnapshot {
  ClientState state{ClientState::closed};
  std::uint64_t frames_received{0U};
  std::uint64_t frames_delivered{0U};
  std::uint64_t frames_dropped{0U};
  std::uint64_t imu_samples_received{0U};
  std::uint64_t sync_messages_received{0U};
  std::uint64_t parse_errors{0U};
  std::uint64_t crc_errors{0U};
  std::uint64_t reconnects{0U};
  std::int64_t last_frame_timestamp_ns{0};
  std::string last_error;
  // Aggregate count of subscription callback exceptions. This is appended to
  // preserve the existing public struct layout prefix.
  std::uint64_t callback_failures{0U};
  std::uint64_t imu_sequence_gaps{0U};
  // Image stream counters by camera 0..3; delivery means callback completion.
  std::array<std::uint64_t, 4> image_delivered{};
  std::array<std::uint64_t, 4> image_dropped{};
  std::array<std::uint64_t, 4> image_crc_errors{};
  // OCPU/1 counters. Received/delivered count only valid CRC-checked samples.
  std::uint64_t temperature_samples_received{0U};
  std::uint64_t temperature_samples_delivered{0U};
  std::uint64_t temperature_samples_dropped{0U};
  std::uint64_t temperature_crc_errors{0U};
  std::uint64_t temperature_sequence_gaps{0U};
};

ORBVI_SDK_API const char* to_string(ClientState state) noexcept;

}  // namespace orbvi_sdk
