#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "orbvi_sdk/contract.hpp"
#include "orbvi_sdk/export.hpp"

namespace orbvi_sdk {

struct FrameView {
  const std::uint8_t* data{nullptr};
  std::size_t size{0U};
  std::uint32_t camera_index{0U};
  std::uint64_t sequence{0U};
  std::uint64_t group_id{0U};
  std::int64_t timestamp_ns{0};
  std::int64_t sensor_timestamp_ns{0};
  std::uint32_t generation{0U};
  std::uint32_t width{0U};
  std::uint32_t height{0U};
  std::uint32_t stride{0U};
  PixelFormat pixel_format{PixelFormat::unknown};
  FrameLayout layout{FrameLayout::unknown};
  ContractFingerprint contract{};
  bool crc_valid{true};

  [[nodiscard]] bool valid() const noexcept {
    return data != nullptr && size > 0U && width > 0U && height > 0U;
  }
};

class ORBVI_SDK_API OwnedFrame {
 public:
  OwnedFrame() = default;
  explicit OwnedFrame(const FrameView& view);

  [[nodiscard]] FrameView view() const noexcept;
  [[nodiscard]] const std::vector<std::uint8_t>& storage() const noexcept {
    return storage_;
  }
  [[nodiscard]] bool empty() const noexcept { return storage_.empty(); }

 private:
  std::vector<std::uint8_t> storage_;
  FrameView metadata_{};
};

struct ImageFrame {
  FrameView pixels{};
  std::uint32_t tile_index{0U};
  bool borrowed{true};

  [[nodiscard]] bool valid() const noexcept { return pixels.valid(); }
};

}  // namespace orbvi_sdk