#pragma once

#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>

#include "orbvi_sdk/export.hpp"

namespace orbvi_sdk {

enum class StatusCode {
  ok = 0,
  invalid_argument,
  not_found,
  unavailable,
  not_supported,
  already_open,
  not_open,
  busy,
  timeout,
  corrupt_data,
  io_error,
  cancelled,
  internal_error,
};

ORBVI_SDK_API const char* to_string(StatusCode code) noexcept;
ORBVI_SDK_API const char* status_code_name(StatusCode code) noexcept;

class ORBVI_SDK_API Status {
 public:
  Status() noexcept = default;
  Status(StatusCode code, std::string message = {});
  Status(const Status&) = default;
  Status(Status&&) noexcept = default;
  Status& operator=(const Status&) = default;
  Status& operator=(Status&&) noexcept = default;
  ~Status() = default;

  [[nodiscard]] StatusCode code() const noexcept { return code_; }
  [[nodiscard]] bool ok() const noexcept { return code_ == StatusCode::ok; }
  [[nodiscard]] explicit operator bool() const noexcept { return ok(); }
  [[nodiscard]] const std::string& message() const noexcept { return message_; }
  [[nodiscard]] std::string to_string() const;

 private:
  StatusCode code_{StatusCode::ok};
  std::string message_;
};

template <typename T>
class Result {
 public:
  Result(T value) : value_(std::move(value)) {}
  Result(Status status) : value_(std::move(status)) {}

  [[nodiscard]] bool ok() const noexcept {
    return std::holds_alternative<T>(value_);
  }
  [[nodiscard]] explicit operator bool() const noexcept { return ok(); }

  [[nodiscard]] T& value() & {
    if (ok()) {
      return std::get<T>(value_);
    }
    throw std::logic_error(std::get<Status>(value_).to_string());
  }
  [[nodiscard]] const T& value() const& {
    if (ok()) {
      return std::get<T>(value_);
    }
    throw std::logic_error(std::get<Status>(value_).to_string());
  }
  [[nodiscard]] T&& value() && {
    if (ok()) {
      return std::move(std::get<T>(value_));
    }
    throw std::logic_error(std::get<Status>(value_).to_string());
  }

  [[nodiscard]] const Status& status() const& {
    static const Status ok_status{};
    return ok() ? ok_status : std::get<Status>(value_);
  }
  [[nodiscard]] Status& status() & {
    return const_cast<Status&>(static_cast<const Result&>(*this).status());
  }

  template <typename U>
  [[nodiscard]] T value_or(U&& fallback) const& {
    return ok() ? std::get<T>(value_) : static_cast<T>(std::forward<U>(fallback));
  }

 private:
  std::variant<T, Status> value_;
};

}  // namespace orbvi_sdk