#pragma once

#include <cstdint>
#include <string>

#include "orbvi_sdk/export.hpp"

namespace orbvi_sdk {

inline constexpr char kProductId4pX5[] = "4p-x5";
inline constexpr char kDefaultReplayFormat[] = "orbvi-replay-v1";
inline constexpr std::uint32_t kDraftProtocolMajor = 0;
inline constexpr std::uint32_t kDraftProtocolMinor = 1;
inline constexpr std::uint32_t kFrameSyncMagic = 0x4f465359U;
inline constexpr std::uint32_t kFrameSyncVersion = 2U;
inline constexpr std::uint32_t kTileWidth = 1280U;
inline constexpr std::uint32_t kTileHeight = 1088U;
inline constexpr std::uint32_t kCameraCount4p = 4U;

enum class PixelFormat : std::uint32_t {
  unknown = 0,
  mjpeg,
  yuyv,
  nv12,
  gray8,
  raw16,
  bgr8,
};

enum class FrameLayout : std::uint32_t {
  unknown = 0,
  single_plane,
};

struct ContractFingerprint {
  std::string product_id{kProductId4pX5};
  std::string layout{"grid_2x2_1280x1088_ports_v1"};
  std::uint32_t protocol_major{kDraftProtocolMajor};
  std::uint32_t protocol_minor{kDraftProtocolMinor};
  std::uint32_t frame_sync_version{kFrameSyncVersion};
  bool frozen{false};

  [[nodiscard]] bool valid() const noexcept {
    return !product_id.empty() && !layout.empty();
  }
};

ORBVI_SDK_API const char* to_string(PixelFormat format) noexcept;
ORBVI_SDK_API const char* to_string(FrameLayout layout) noexcept;

}  // namespace orbvi_sdk
