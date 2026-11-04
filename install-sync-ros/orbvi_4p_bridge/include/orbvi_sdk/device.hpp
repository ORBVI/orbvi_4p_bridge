#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "orbvi_sdk/contract.hpp"
#include "orbvi_sdk/export.hpp"

namespace orbvi_sdk {

enum class DeviceCapability {
  imu,
  frame_sync,
  timing_diagnostics,
  control,
};

using Capability = DeviceCapability;

struct DeviceSelector {
  std::string serial;
  std::string product_id{kProductId4pX5};
  std::uint32_t index{0U};
  bool select_any{false};
};

struct DeviceInfo {
  std::string id;
  std::string serial;
  std::string product_id;
  std::string firmware_version;
  std::uint16_t vendor_id{0U};
  std::uint16_t product_id_number{0U};
  std::uint8_t bus_number{0U};
  std::uint8_t device_address{0U};
  bool fake{false};
  std::vector<DeviceCapability> capabilities;
};

ORBVI_SDK_API const char* to_string(DeviceCapability capability) noexcept;
ORBVI_SDK_API bool has_capability(const DeviceInfo& device,
                                  DeviceCapability capability) noexcept;

}  // namespace orbvi_sdk