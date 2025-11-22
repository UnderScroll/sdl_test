#pragma once

#include <algorithm>
#include <expected>
#include <format>
#include <memory>
#include <optional>
#include <source_location>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

#include "magic_enum/magic_enum.hpp"

namespace re {

class IError; // Predefined for AnyError
using AnyError = std::unique_ptr<IError>;

template <typename TKind> // Predefined for IError::kind()
  requires std::is_enum_v<TKind>
class Error;

// Generic abstract Error Interface
class IError {
 public:
  /* Pure virtual functions */
  virtual const std::source_location& location() const noexcept = 0;
  virtual std::string_view message() const noexcept = 0;
  virtual const std::optional<AnyError>& cause() const noexcept = 0;
  constexpr virtual std::string_view kind_name() const noexcept = 0;

  /* Public functions */
  template <typename TKind>
    requires std::is_enum_v<TKind>
  const TKind kind() const {
    if (const Error<TKind>* error = dynamic_cast<const Error<TKind>*>(this)) {
      return error->kind();
    } else {
      throw std::runtime_error(std::format("Can't downcast this [AnyError] to an [Error<{}>].", typeid(TKind).name()));
    }
  }

  /* Virtual Destructor */
  virtual ~IError() = default;
};

// Typed Error
template <typename TKind>
  requires std::is_enum_v<TKind>
class Error : public IError {
 protected:
  /* Private members */
  // Enumerable kind of error (User defined)
  TKind _kind;
  // User defined message, used to explain what happened
  std::string _message{};
  // Optional cause if it was the result of an other error
  std::optional<AnyError> _cause{};
  // Location where the error was encountered
  std::source_location _location;

  /* Constructors (Protected, use functional constructors instead) */
  Error(const TKind kind, const std::string message, const std::source_location location)
      : _kind{kind}, _message{message}, _cause{}, _location{location} {}
  Error(const TKind kind, const std::string message, AnyError&& cause, const std::source_location location)
      : _kind{kind}, _message{message}, _cause{std::move(cause)}, _location{location} {}

 public:
  /* Special Contructors */
  // AnyError not copyable => Error not copyable
  Error(const Error&) = delete;
  Error& operator=(const Error&) = delete;

  // Error can be moved
  Error(Error&&) = default;
  Error& operator=(Error&&) = default;

  /* Getters */
  const TKind kind() const {
    return _kind;
  }

  constexpr std::string_view kind_name() const noexcept override {
    return magic_enum::enum_name(_kind);
  }

  const std::source_location& location() const noexcept override {
    return _location;
  }

  std::string_view message() const noexcept override {
    return _message;
  }

  const std::optional<AnyError>& cause() const noexcept override {
    return _cause;
  }

  /* Friends */
  template <typename UKind>
    requires std::is_enum_v<UKind>
  friend Error<UKind> error(UKind&& kind, std::string&& message, std::source_location location) noexcept;

  template <typename UKind>
    requires std::is_enum_v<UKind>
  friend Error<UKind> error(UKind&& kind, std::string&& message, AnyError&& cause, std::source_location location) noexcept;

  template <typename UKindA, typename UKindB>
    requires(std::is_enum_v<UKindA>, std::is_enum_v<UKindB>)
  friend Error<UKindA> error(UKindA&& kind, std::string&& message, Error<UKindB>&& cause, std::source_location location) noexcept;

  template <typename UKind>
    requires std::is_enum_v<UKind>
  friend AnyError anyError(UKind&& kind, std::string&& message, std::source_location location) noexcept;

  template <typename UKind>
    requires std::is_enum_v<UKind>
  friend AnyError anyError(UKind&& kind, std::string&& message, AnyError&& cause, std::source_location location) noexcept;

  template <typename UKindA, typename UKindB>
    requires(std::is_enum_v<UKindA>, std::is_enum_v<UKindB>)
  friend AnyError anyError(UKindA&& kind, std::string&& message, Error<UKindB>&& cause, std::source_location location) noexcept;

  template <typename UKind>
    requires std::is_enum_v<UKind>
  friend AnyError anyError(Error<UKind>&& error) noexcept;
};

/* Functional contructors */
template <typename UKind>
  requires std::is_enum_v<UKind>
[[nodiscard]]
Error<UKind> error(UKind&& kind, std::string&& message, std::source_location location = std::source_location::current()) noexcept {
  return Error<UKind>{std::forward<UKind>(kind), std::forward<std::string>(message), location};
}

template <typename UKind>
  requires std::is_enum_v<UKind>
[[nodiscard]]
Error<UKind> error(UKind&& kind, std::string&& message, AnyError&& cause, std::source_location location = std::source_location::current()) noexcept {
  return Error<UKind>{std::forward<UKind>(kind), std::forward<std::string>(message), std::move(cause), location};
}

template <typename UKindA, typename UKindB>
  requires(std::is_enum_v<UKindA>, std::is_enum_v<UKindB>)
[[nodiscard]]
Error<UKindA> error(UKindA&& kind, std::string&& message, Error<UKindB>&& cause, std::source_location location = std::source_location::current()) noexcept {
  return Error<UKindA>{std::forward<UKindA>(kind), std::forward<std::string>(message), anyError(std::forward<Error<UKindB>>(cause)), location};
}

template <typename UKind>
  requires std::is_enum_v<UKind>
[[nodiscard]]
AnyError anyError(UKind&& kind, std::string&& message, std::source_location location = std::source_location::current()) noexcept {
  return std::unique_ptr<Error<UKind>>{new Error<UKind>{std::forward<UKind>(kind), std::forward<std::string>(message), location}};
}

template <typename UKind>
  requires std::is_enum_v<UKind>
[[nodiscard]]
AnyError anyError(UKind&& kind, std::string&& message, AnyError&& cause, std::source_location location = std::source_location::current()) noexcept {
  return std::unique_ptr<Error<UKind>>{new Error<UKind>{std::forward<UKind>(kind), std::forward<std::string>(message), std::move(cause), location}};
}

template <typename UKindA, typename UKindB>
  requires(std::is_enum_v<UKindA>, std::is_enum_v<UKindB>)
[[nodiscard]]
AnyError anyError(UKindA&& kind, std::string&& message, Error<UKindB>&& cause, std::source_location location = std::source_location::current()) noexcept {
  return std::unique_ptr<Error<UKindA>>{new Error<UKindA>{std::forward<UKindA>(kind), std::forward<std::string>(message), std::make_unique<Error<UKindB>>(std::move(cause)), location}};
}

template <typename UKind>
  requires std::is_enum_v<UKind>
[[nodiscard]]
AnyError anyError(Error<UKind>&& error) noexcept {
  return std::unique_ptr<Error<UKind>>{new Error<UKind>{std::move(error)}};
}

/* Quick std::expected<void, re::Error<TKind>> */
template <typename TError>
  requires(std::derived_from<TError, re::IError> || std::is_class_v<AnyError>)
using expected = std::expected<void, TError>;

} // namespace re
