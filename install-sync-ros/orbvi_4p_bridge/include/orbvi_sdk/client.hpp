#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "orbvi_sdk/compressed_frame.hpp"
#include "orbvi_sdk/contract.hpp"
#include "orbvi_sdk/device.hpp"
#include "orbvi_sdk/diagnostics.hpp"
#include "orbvi_sdk/export.hpp"
#include "orbvi_sdk/frame.hpp"
#include "orbvi_sdk/imu.hpp"
#include "orbvi_sdk/options.hpp"
#include "orbvi_sdk/status.hpp"
#include "orbvi_sdk/subscription.hpp"
#include "orbvi_sdk/sync.hpp"
#include "orbvi_sdk/temperature_stream.hpp"

namespace orbvi_sdk {

namespace detail {
class Runtime;
struct ClientStateData;
}

using FrameCallback = std::function<void(const FrameView&)>;
using CompressedFrameCallback = std::function<void(const CompressedFrameView&)>;
using TemperatureCallback = std::function<void(const TemperatureSample&)>;
using ImuCallback = std::function<void(const ImuEvent&)>;
using SyncCallback = std::function<void(const FrameSync&)>;
using DiagnosticsCallback = std::function<void(const DiagnosticsSnapshot&)>;

struct CallbackSet {
  // Existing fields stay first so positional aggregate initialization remains
  // source compatible. New image/temperature callbacks are appended.
  FrameCallback on_frame;
  ImuCallback on_imu;
  SyncCallback on_sync;
  DiagnosticsCallback on_diagnostics;
  CompressedFrameCallback on_compressed_frame;
  TemperatureCallback on_temperature;
};

class ORBVI_SDK_API Client {
 public:
  explicit Client(ClientOptions options = {});
  Client(const Client&) = delete;
  Client& operator=(const Client&) = delete;
  Client(Client&& other) noexcept;
  Client& operator=(Client&& other) noexcept;
  ~Client();

  [[nodiscard]] const ClientOptions& options() const noexcept;
  [[nodiscard]] ClientState state() const noexcept;
  [[nodiscard]] DiagnosticsSnapshot diagnostics() const;

  Result<std::vector<DeviceInfo>> enumerate() const;

  Status open();
  Status start();
  Status stop();
  Status close() noexcept;

  Result<SubscriptionHandle> subscribe(SubscribeOptions options,
                                       CallbackSet callbacks);

 private:
  std::unique_ptr<detail::ClientStateData> state_;
};

ORBVI_SDK_API const char* sdk_version() noexcept;

}  // namespace orbvi_sdk