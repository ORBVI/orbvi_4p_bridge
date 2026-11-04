#pragma once

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <memory>

#include "orbvi_sdk/export.hpp"
#include "orbvi_sdk/options.hpp"

namespace orbvi_sdk {

namespace detail {
struct SubscriptionState;
}

struct SubscriptionStats {
  std::uint64_t delivered{0U};
  std::uint64_t dropped{0U};
  std::uint64_t failed{0U};
  std::size_t queued{0U};
  std::size_t queue_capacity{0U};
  bool cancelled{false};
};

class ORBVI_SDK_API SubscriptionHandle {
 public:
  SubscriptionHandle() noexcept = default;
  SubscriptionHandle(const SubscriptionHandle&) = default;
  SubscriptionHandle(SubscriptionHandle&&) noexcept = default;
  SubscriptionHandle& operator=(const SubscriptionHandle&) = default;
  SubscriptionHandle& operator=(SubscriptionHandle&&) noexcept = default;
  ~SubscriptionHandle() = default;

  [[nodiscard]] bool valid() const noexcept;
  void cancel() noexcept;
  [[nodiscard]] SubscriptionStats stats() const;
  [[nodiscard]] bool wait_for_delivery(
      std::uint64_t target_count,
      std::chrono::milliseconds timeout = std::chrono::milliseconds{1000}) const;

 private:
  friend class Client;
  explicit SubscriptionHandle(std::shared_ptr<detail::SubscriptionState> state);

  std::shared_ptr<detail::SubscriptionState> state_;
};

}  // namespace orbvi_sdk