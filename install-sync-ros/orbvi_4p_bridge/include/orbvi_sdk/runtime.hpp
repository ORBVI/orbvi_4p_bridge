#pragma once

#include "orbvi_sdk/export.hpp"

namespace orbvi_sdk {

// Runtime configuration is injected through ClientOptions. Concrete
// configurations live in fake.hpp and replay.hpp so core users never depend
// on the hardware backend implementation.
class ORBVI_SDK_API RuntimeConfig {
 public:
  RuntimeConfig() = default;
  RuntimeConfig(const RuntimeConfig&) = default;
  RuntimeConfig(RuntimeConfig&&) noexcept = default;
  RuntimeConfig& operator=(const RuntimeConfig&) = default;
  RuntimeConfig& operator=(RuntimeConfig&&) noexcept = default;
  virtual ~RuntimeConfig();
};

}  // namespace orbvi_sdk