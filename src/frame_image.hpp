#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>
#include <string>

#include <sensor_msgs/msg/image.hpp>

#include "orbvi_sdk/frame.hpp"

namespace orbvi_bridge {

inline bool frame_to_image(const orbvi_sdk::FrameView& frame,
                           sensor_msgs::msg::Image& message,
                           std::string& error) {
  if (!frame.valid() || frame.layout != orbvi_sdk::FrameLayout::single_plane) {
    error = "invalid frame or unsupported layout";
    return false;
  }
  std::size_t channels = 0U;
  if (frame.pixel_format == orbvi_sdk::PixelFormat::gray8) {
    channels = 1U;
  } else if (frame.pixel_format == orbvi_sdk::PixelFormat::bgr8) {
    channels = 3U;
  } else {
    error = "unsupported pixel format";
    return false;
  }

  const auto width = static_cast<std::size_t>(frame.width);
  const auto height = static_cast<std::size_t>(frame.height);
  const auto stride = static_cast<std::size_t>(frame.stride);
  const auto maximum = std::numeric_limits<std::size_t>::max();
  if (width > maximum / channels) {
    error = "frame row size overflows";
    return false;
  }
  const std::size_t row_bytes = width * channels;
  if (row_bytes > std::numeric_limits<std::uint32_t>::max() || stride < row_bytes ||
      height > maximum / row_bytes ||
      height - 1U > (maximum - row_bytes) / stride) {
    error = "invalid frame stride or size";
    return false;
  }
  const std::size_t required_input = (height - 1U) * stride + row_bytes;
  if (frame.size < required_input) {
    error = "truncated frame";
    return false;
  }

  message.width = frame.width;
  message.height = frame.height;
  message.encoding = channels == 3U ? "bgr8" : "mono8";
  message.is_bigendian = 0U;
  message.step = static_cast<std::uint32_t>(row_bytes);
  message.data.resize(row_bytes * height);
  for (std::size_t row = 0U; row < height; ++row) {
    std::memcpy(message.data.data() + row * row_bytes,
                frame.data + row * stride, row_bytes);
  }
  return true;
}

}
