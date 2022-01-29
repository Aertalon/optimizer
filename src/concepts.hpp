#pragma once

#include <concepts>

namespace opt {

// clang-format off
template <class T, class U = T, class R = T>
concept Addable = requires (T a, U b) { { a + b } -> std::same_as<R>; };

template <class T, class U = T, class R = T>
concept Subtractible = requires (T a, U b) { { a - b } -> std::same_as<R>; };

template <class T, class U = T, class R = T>
concept Multipliable = requires (T a, U b) { { a * b } -> std::same_as<R>; };

template <class T, class U = T, class R = T>
concept Divisible = requires (T a, U b) { { a / b } -> std::same_as<R>; };

template <class T>
concept Negatable = requires (T a) { { -a  } -> std::same_as<T>; };

template <class T>
concept Arithmetic =
  std::regular<T> &&
  Addable<T> &&
  Subtractible<T> &&
  Multipliable<T> &&
  Divisible<T> &&
  Negatable<T> &&
  requires(T a) {
    T{0};  // additive identity
    T{1};  // multiplicative identity
  };
// clang-format on

}  // namespace opt
