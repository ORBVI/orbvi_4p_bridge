#pragma once

#include <chrono>
#include <cstdint>
#include <string>

#include "orbvi_sdk/export.hpp"
#include "orbvi_sdk/runtime.hpp"

namespace orbvi_sdk {

class Client;

// OIMG/1 kind values. NV12 is delivered through on_frame and JPEG through
// on_compressed_frame.
enum class ImageStreamFormat : std::uint8_t {
  nv12 = 1U,
  jpeg = 2U,
};

struct ImageStreamConfig final : RuntimeConfig {
  // Board IPv4 address. The SDK is the TCP client and the board is the server.
  std::string host;
  std::uint16_t base_port{5610U};
  std::uint8_t camera_mask{0x0fU};
  std::uint32_t frames_per_camera{10U};
  ImageStreamFormat format{ImageStreamFormat::nv12};
  std::chrono::milliseconds connect_timeout{10000};
  std::chrono::milliseconds read_timeout{12000};
};

ORBVI_SDK_API Client make_image_stream_client(const ImageStreamConfig& config);

}  // namespace orbvi_sdk
