#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include "orbvi_sdk/export.hpp"

namespace orbvi_sdk {

// data belongs to the callback; copy it if it must outlive the callback.
struct CompressedFrameView {
  const std::uint8_t* data{nullptr};
  std::size_t size{0U};
  std::uint32_t camera_index{0U};
  std::uint32_t width{0U};
  std::uint32_t height{0U};
  std::uint64_t sequence{0U};
  std::int64_t timestamp_ns{0};         // host receive clock
  std::int64_t sensor_timestamp_ns{0};  // board clock, not synchronized
  bool crc_valid{true};
};

class ORBVI_SDK_API OwnedCompressedFrame {
 public:
  OwnedCompressedFrame() = default;
  explicit OwnedCompressedFrame(const CompressedFrameView& view);

  [[nodiscard]] CompressedFrameView view() const noexcept;
  [[nodiscard]] const std::vector<std::uint8_t>& storage() const noexcept {
    return storage_;
  }
  [[nodiscard]] bool empty() const noexcept { return storage_.empty(); }

 private:
  std::vector<std::uint8_t> storage_;
  CompressedFrameView metadata_{};
};

}  // namespace orbvi_sdk
