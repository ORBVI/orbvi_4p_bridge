#include <cassert>
#include <cstdint>
#include <limits>

#include "capture_statistics.hpp"

int main() {
  orbvi_bridge::CaptureStatistics delayed;
  delayed.add_data(50, true, false);
  delayed.add_data(100, true, false);
  delayed.add_metadata(100, 10, true, true);
  delayed.add_data(200, true, true);
  delayed.add_data(300, true, true);
  assert(!delayed.complete(200));
  delayed.add_metadata(200, 11, true, true);
  delayed.add_metadata(300, 12, true, true);
  assert(delayed.complete(200));
  auto result = delayed.summarize(200, true);
  assert(result.count == 2 && result.metadata_count == 2);
  assert(!result.gaps && !result.discontinuities && !result.invalid);
  assert(!result.missing_metadata && !result.missing_data);

  orbvi_bridge::CaptureStatistics missing;
  missing.add_data(100, true, true);
  missing.add_data(200, true, true);
  missing.add_metadata(100, 10, true, true);
  missing.add_data(300, true, true);
  missing.add_metadata(300, 12, true, true);
  assert(!missing.complete(200));
  result = missing.summarize(200, true);
  assert(result.count == 2 && result.metadata_count == 1);
  assert(result.missing_metadata == 1);
  assert(result.first_missing_metadata == 200);
  assert(result.window_start == 100 && result.window_end == 300);
  missing.add_metadata(200, 11, true, true);
  assert(missing.complete(200));

  orbvi_bridge::CaptureStatistics gap;
  for (const auto stamp : {100, 200, 300}) gap.add_data(stamp, true, true);
  gap.add_metadata(100, 20, true, true);
  gap.add_metadata(200, 22, true, true);
  gap.add_metadata(300, 23, true, true);
  assert(gap.summarize(200, true).gaps == 1);

  orbvi_bridge::CaptureStatistics wrapped;
  wrapped.add_data(100, true, true);
  wrapped.add_data(200, true, true);
  wrapped.add_metadata(100, 0xffffffffU, true, true);
  wrapped.add_metadata(200, 0U, true, true);
  result = wrapped.summarize(200, true);
  assert(!result.gaps && !result.discontinuities);
  assert(wrapped.summarize(200, false).discontinuities == 1);

  orbvi_bridge::CaptureStatistics duplicates;
  duplicates.add_data(100, true, true);
  duplicates.add_data(100, true, true);
  duplicates.add_metadata(100, 1, true, true);
  result = duplicates.summarize(200, true);
  assert(result.missing_metadata == 1 && result.discontinuities == 1);

  orbvi_bridge::CaptureStatistics invalid;
  invalid.add_data(0, false, true);
  invalid.add_data(100, false, true);
  invalid.add_metadata(100, 1, true, true);
  assert(invalid.summarize(200, true).invalid == 2);

  orbvi_bridge::CaptureStatistics overflow;
  overflow.add_data(std::numeric_limits<std::int64_t>::max(), true, true);
  assert(!overflow.complete(200));
  assert(overflow.summarize(200, true).invalid == 1);
}
