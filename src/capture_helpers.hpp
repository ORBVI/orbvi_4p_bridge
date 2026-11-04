#pragma once

#include <cstdint>
#include <limits>

#include <builtin_interfaces/msg/time.hpp>

namespace orbvi_bridge {

inline bool timestamp_from_ns(std::int64_t value, builtin_interfaces::msg::Time& stamp) {
  constexpr std::int64_t kBillion = 1000000000LL;
  if (value <= 0 || value / kBillion > std::numeric_limits<std::int32_t>::max()) {
    return false;
  }
  stamp.sec = static_cast<std::int32_t>(value / kBillion);
  stamp.nanosec = static_cast<std::uint32_t>(value % kBillion);
  return true;
}

}
