#pragma once

#include <cstdint>
#include <string>

#include "orbvi_sdk/export.hpp"
#include "orbvi_sdk/runtime.hpp"

namespace orbvi_sdk {

class Client;

enum class ReplayMode {
  as_fast_as_possible,
  preserve_timestamp_delta,
};

struct ReplayConfig final : RuntimeConfig {
  std::string path;
  ReplayMode mode{ReplayMode::as_fast_as_possible};
  std::uint64_t speed_limit_us{0U};
  bool stop_at_end{true};
};

ORBVI_SDK_API Client make_replay_client(const ReplayConfig& config);

}  // namespace orbvi_sdk