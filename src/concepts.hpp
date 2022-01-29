#pragma once

#include <concepts>
#include <cstddef>

namespace opt {

// clang-format off
template <class T, class U = T, class R = T>
concept Addable =
  requires (T a, U b) { { a + b } -> std::same_as<R>; };

template <class T, class U = T, class R = T>
concept Subtractible =
  requires (T a, U b) { { a - b } -> std::same_as<R>; };

template <class T, class U = T, class R = T>
concept Multipliable =
  requires (T a, U b) { { a * b } -> std::same_as<R>; };

template <class T, class U = T, class R = T>
concept Divisible =
  requires (T a, U b) { { a / b } -> std::same_as<R>; };

template <class T>
concept Negatable =
  requires (T a) { { -a  } -> std::same_as<T>; };

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

namespace detail {

template <class T>
concept is_lvalue_ref = std::is_lvalue_reference_v<T>;

template <class T>
concept is_const_lvalue_ref =
  std::is_lvalue_reference_v<T> &&
  std::is_const_v<std::remove_reference_t<T>>;

}  // namespace detail

template <class T, class U = std::size_t>
concept Indexable =
  requires(T& a, const T& b, U n) {
    { a[n] } -> detail::is_lvalue_ref;
    { b[n] } -> detail::is_const_lvalue_ref;
  };

template <Indexable T>
using scalar_t = std::remove_cvref_t<decltype(std::declval<T&>()[0])>;

template <class T, class U = scalar_t<T>>
concept Vector =
  std::regular<T> &&
  Indexable<T> &&
  Addable<T> &&
  Subtractible<T> &&
  Negatable<T> &&
  Multipliable<U, const T&, T>;

template <class T>
concept Diffable =
  requires (T a, T b) { a - b; };

template <Diffable T>
using distance_t =
  std::remove_cvref_t<decltype(std::declval<const T&>() - std::declval<const T&>())>;

template <class T, class U = distance_t<T>>
concept Point =
  std::regular<T> &&
  Indexable<T> &&
  Addable<const T&, const U&, T> &&
  Addable<const U&, const T&, T> &&
  Subtractible<const T&, const T&, U> &&
  Subtractible<const T&, const U&, T>;

// clang-format on

}  // namespace opt
