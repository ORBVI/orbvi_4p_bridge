#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

#include "orbvi_sdk/export.hpp"
#include "orbvi_sdk/imu.hpp"
#include "orbvi_sdk/runtime.hpp"
#include "orbvi_sdk/status.hpp"

namespace orbvi_sdk {
class Client;

// IPv4 UDP listener; the sender uses the fixed 72-byte OIMU/1 datagram.
struct UdpImuConfig final : RuntimeConfig {
  std::string bind_address{"0.0.0.0"};
  std::uint16_t port{5600U};
  // Empty means accept any source; use the board's IPv4 address to restrict it.
  std::string source_address;
};

// Packet timestamps are raw IIO/board clock domains, NOT Unix time.
// host_receive_ns is the host's CLOCK_REALTIME timestamp at receive().
ORBVI_SDK_API Result<ImuSample> parse_oimu_packet(
    const std::uint8_t* data, std::size_t size, std::int64_t host_receive_ns);
ORBVI_SDK_API Client make_udp_imu_client(const UdpImuConfig& config);
}  // namespace orbvi_sdk
