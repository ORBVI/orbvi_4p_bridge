#pragma once

#include <algorithm>
#include <cstdint>
#include <limits>
#include <mutex>
#include <unordered_map>
#include <vector>

namespace orbvi_bridge {

struct CaptureSummary {
  std::uint64_t count{0U};
  std::uint64_t metadata_count{0U};
  std::uint64_t gaps{0U};
  std::uint64_t discontinuities{0U};
  std::uint64_t invalid{0U};
  std::uint64_t missing_metadata{0U};
  std::uint64_t missing_data{0U};
  std::int64_t window_start{0};
  std::int64_t window_end{0};
  std::int64_t first_missing_data{0};
  std::int64_t first_missing_metadata{0};
};

class CaptureStatistics {
 public:
  void add_data(std::int64_t stamp, bool valid, bool begin_window) {
    const std::lock_guard<std::mutex> lock(mutex_);
    observe(stamp, begin_window);
    data_.push_back({stamp, 0U, valid});
    latest_data_ = std::max(latest_data_, stamp);
  }

  void add_metadata(std::int64_t stamp, std::uint64_t sequence, bool valid,
                    bool begin_window) {
    const std::lock_guard<std::mutex> lock(mutex_);
    observe(stamp, begin_window);
    metadata_.push_back({stamp, sequence, valid});
    latest_metadata_ = std::max(latest_metadata_, stamp);
  }

  bool complete(std::int64_t duration_ns, bool sequence_32bit = false) const {
    const std::lock_guard<std::mutex> lock(mutex_);
    if (!valid_window(duration_ns) || latest_data_ < start_ + duration_ns ||
        latest_metadata_ < start_ + duration_ns) return false;
    const auto result = summarize_locked(duration_ns, sequence_32bit);
    return result.missing_metadata == 0U && result.missing_data == 0U;
  }

  CaptureSummary summarize(std::int64_t duration_ns, bool sequence_32bit) const {
    const std::lock_guard<std::mutex> lock(mutex_);
    return summarize_locked(duration_ns, sequence_32bit);
  }

 private:
  CaptureSummary summarize_locked(std::int64_t duration_ns, bool sequence_32bit) const {
    CaptureSummary result;
    result.invalid = invalid_stamps_;
    if (!valid_window(duration_ns)) {
      ++result.invalid;
      return result;
    }
    const auto end = start_ + duration_ns;
    result.window_start = start_;
    result.window_end = end;
    std::unordered_map<std::int64_t, std::uint64_t> data_counts;
    std::unordered_map<std::int64_t, std::uint64_t> metadata_counts;
    std::int64_t previous_stamp = 0;
    for (const auto& record : data_) {
      if (record.stamp < start_ || record.stamp >= end) continue;
      ++result.count;
      ++data_counts[record.stamp];
      if (!record.valid) ++result.invalid;
      if (previous_stamp && record.stamp <= previous_stamp) ++result.discontinuities;
      previous_stamp = record.stamp;
    }
    previous_stamp = 0;
    std::uint64_t previous_sequence = 0U;
    bool seen = false;
    for (const auto& record : metadata_) {
      if (record.stamp < start_ || record.stamp >= end) continue;
      ++result.metadata_count;
      ++metadata_counts[record.stamp];
      if (!record.valid) ++result.invalid;
      if (previous_stamp && record.stamp <= previous_stamp) ++result.discontinuities;
      if (seen) {
        if (sequence_32bit) {
          const auto delta = static_cast<std::uint32_t>(record.sequence - previous_sequence);
          if (delta == 0U || delta >= 0x80000000U) ++result.discontinuities;
          else if (delta > 1U) result.gaps += delta - 1U;
        } else if (record.sequence <= previous_sequence) {
          ++result.discontinuities;
        } else {
          result.gaps += record.sequence - previous_sequence - 1U;
        }
      }
      previous_stamp = record.stamp;
      previous_sequence = record.sequence;
      seen = true;
    }
    for (const auto& entry : data_counts) {
      const auto found = metadata_counts.find(entry.first);
      const auto matched = found == metadata_counts.end() ? 0U : found->second;
      if (entry.second > matched) {
        result.missing_metadata += entry.second - matched;
        if (!result.first_missing_metadata || entry.first < result.first_missing_metadata)
          result.first_missing_metadata = entry.first;
      }
    }
    for (const auto& entry : metadata_counts) {
      const auto found = data_counts.find(entry.first);
      const auto matched = found == data_counts.end() ? 0U : found->second;
      if (entry.second > matched) {
        result.missing_data += entry.second - matched;
        if (!result.first_missing_data || entry.first < result.first_missing_data)
          result.first_missing_data = entry.first;
      }
    }
    return result;
  }

 private:
  struct Record {
    std::int64_t stamp;
    std::uint64_t sequence;
    bool valid;
  };

  void observe(std::int64_t stamp, bool begin_window) {
    if (!begin_window) return;
    if (stamp <= 0) ++invalid_stamps_;
    else if (start_ == 0) start_ = stamp;
  }

  bool valid_window(std::int64_t duration_ns) const {
    return start_ > 0 && duration_ns > 0 &&
           start_ <= std::numeric_limits<std::int64_t>::max() - duration_ns;
  }

  std::int64_t start_{0};
  std::int64_t latest_data_{0};
  std::int64_t latest_metadata_{0};
  std::uint64_t invalid_stamps_{0U};
  std::vector<Record> data_;
  std::vector<Record> metadata_;
  mutable std::mutex mutex_;
};

}
