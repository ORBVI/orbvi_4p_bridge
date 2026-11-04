#include <cstdint>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "frame_image.hpp"

int main() {
  const std::vector<std::uint8_t> input{
      1, 2, 3, 4, 5, 6, 99, 99, 7, 8, 9, 10, 11, 12};
  orbvi_sdk::FrameView frame;
  frame.data = input.data();
  frame.size = input.size();
  frame.width = 2U;
  frame.height = 2U;
  frame.stride = 8U;
  frame.pixel_format = orbvi_sdk::PixelFormat::bgr8;
  frame.layout = orbvi_sdk::FrameLayout::single_plane;
  sensor_msgs::msg::Image image;
  std::string error;
  const std::vector<std::uint8_t> color{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
  if (!orbvi_bridge::frame_to_image(frame, image, error) || image.encoding != "bgr8" ||
      image.width != 2U || image.height != 2U || image.step != 6U || image.data != color) {
    std::cerr << "padded BGR8 conversion failed: " << error << '\n';
    return 1;
  }

  frame.pixel_format = orbvi_sdk::PixelFormat::gray8;
  frame.stride = 3U;
  frame.size = 5U;
  const std::vector<std::uint8_t> gray{1, 2, 4, 5};
  if (!orbvi_bridge::frame_to_image(frame, image, error) || image.encoding != "mono8" ||
      image.step != 2U || image.data != gray) {
    std::cerr << "padded gray8 conversion failed: " << error << '\n';
    return 1;
  }

  frame.pixel_format = orbvi_sdk::PixelFormat::bgr8;
  frame.stride = 8U;
  frame.size = 13U;
  if (orbvi_bridge::frame_to_image(frame, image, error)) {
    std::cerr << "truncated color frame accepted\n";
    return 1;
  }
  frame.size = input.size();
  frame.stride = 5U;
  if (orbvi_bridge::frame_to_image(frame, image, error)) {
    std::cerr << "short stride accepted\n";
    return 1;
  }
  frame.stride = 8U;
  frame.pixel_format = orbvi_sdk::PixelFormat::nv12;
  if (orbvi_bridge::frame_to_image(frame, image, error)) {
    std::cerr << "unsupported format accepted\n";
    return 1;
  }
  frame.pixel_format = orbvi_sdk::PixelFormat::bgr8;
  frame.width = std::numeric_limits<std::uint32_t>::max();
  if (orbvi_bridge::frame_to_image(frame, image, error)) {
    std::cerr << "overflowing row accepted\n";
    return 1;
  }
  frame.width = 2U;
  frame.layout = orbvi_sdk::FrameLayout::unknown;
  if (orbvi_bridge::frame_to_image(frame, image, error)) {
    std::cerr << "unsupported layout accepted\n";
    return 1;
  }
  frame.layout = orbvi_sdk::FrameLayout::single_plane;
  frame.data = nullptr;
  if (orbvi_bridge::frame_to_image(frame, image, error)) {
    std::cerr << "null frame accepted\n";
    return 1;
  }
  return 0;
}
