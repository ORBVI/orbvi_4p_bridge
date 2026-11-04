#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

#include "orbvi_sdk/export.hpp"
#include "orbvi_sdk/runtime.hpp"
#include "orbvi_sdk/status.hpp"

namespace orbvi_sdk {

class Client;

struct TemperatureSample {
  float celsius{0.0F};
  std::uint64_t sequence{0U};
  // Board CLOCK_MONOTONIC timestamp in nanoseconds. Metadata only; it is not a
  // Unix/ROS timestamp and must not be used as the ROS message stamp.
  std::uint64_t monotonic_ns{0U};
};

struct TemperatureStreamConfig final : RuntimeConfig {
  std::string bind_address{"0.0.0.0"};
  // Independent from OIMU/1 (5600) and image streams (5610..5613).
  std::uint16_t port{5620U};
  // 0 receives until stop(); a positive value stops after this many valid,
  // CRC-checked samples.
  std::uint32_t samples{0U};
};

// Parses one complete 32-byte OCPU/1 datagram. Bad magic/version/type/reserved,
// CRC mismatch, non-finite temperature, and wrong lengths are rejected.
ORBVI_SDK_API Result<TemperatureSample> parse_ocpu_packet(
    const std::uint8_t* data, std::size_t size);

ORBVI_SDK_API Client make_temperature_stream_client(
    const TemperatureStreamConfig& config);

}  // namespace orbvi_sdk
