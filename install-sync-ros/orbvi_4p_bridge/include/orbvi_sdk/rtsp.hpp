#pragma once

#include <array>
#include <chrono>
#include <cstdint>
#include <string>

#include "orbvi_sdk/contract.hpp"
#include "orbvi_sdk/export.hpp"
#include "orbvi_sdk/runtime.hpp"

namespace orbvi_sdk {

class Client;

struct RtspConfig final : RuntimeConfig {
  // urls[i] is always associated with camera_index=i. The runtime must not
  // reorder URLs or derive camera indices from URL contents.
  std::array<std::string, 4U> urls{};
  std::chrono::milliseconds connect_timeout{5000};
  std::chrono::milliseconds read_timeout{5000};
  std::chrono::milliseconds reconnect_delay{1000};
  std::uint32_t max_reconnect_attempts{5U};
};

ORBVI_SDK_API Client make_rtsp_client(const RtspConfig& config);
ORBVI_SDK_API Client make_rtsp_client(const RtspConfig& config,
                                     PixelFormat output_format);

}  // namespace orbvi_sdk
