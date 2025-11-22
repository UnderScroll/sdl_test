#pragma once

#include <type_traits>

// Template to specialize to enable bitmask operation
template <typename E>
struct enable_bitmask_operators {
  static constexpr bool enable = false;
};

template <typename E>
concept BitmaskEnum = enable_bitmask_operators<E>::enable;

// Bitmask operations
template <typename E>
  requires BitmaskEnum<E>
constexpr E operator|(E lhs, E rhs) noexcept {
  using Underlying = std::underlying_type_t<E>;
  return static_cast<E>(static_cast<Underlying>(lhs) | static_cast<Underlying>(rhs));
}

template <typename E>
  requires BitmaskEnum<E>
constexpr E operator&(E lhs, E rhs) noexcept {
  using Underlying = std::underlying_type_t<E>;
  return static_cast<E>(static_cast<Underlying>(lhs) & static_cast<Underlying>(rhs));
}

template <typename E>
  requires BitmaskEnum<E>
constexpr E operator~(E flag) noexcept {
  using Underlying = std::underlying_type_t<E>;
  return static_cast<E>(~static_cast<Underlying>(flag));
}
