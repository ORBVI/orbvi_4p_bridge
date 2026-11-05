#include <cassert>
#include <cstdint>
#include <limits>

#include "capture_helpers.hpp"

int main() {
  builtin_interfaces::msg::Time stamp;
  assert(!orbvi_bridge::timestamp_from_ns(0, stamp));
  assert(!orbvi_bridge::timestamp_from_ns(-1, stamp));
  assert(!orbvi_bridge::timestamp_from_ns(std::numeric_limits<std::int64_t>::max(), stamp));
  assert(orbvi_bridge::timestamp_from_ns(12345678901LL, stamp));
  assert(stamp.sec == 12 && stamp.nanosec == 345678901U);

}
