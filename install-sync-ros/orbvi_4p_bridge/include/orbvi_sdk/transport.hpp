#pragma once

#include "orbvi_sdk/export.hpp"

namespace orbvi_sdk {

enum class TransportMode {
  automatic,
  fake,
  replay,
  rtsp,
};

ORBVI_SDK_API const char* to_string(TransportMode mode) noexcept;

}  // namespace orbvi_sdk
