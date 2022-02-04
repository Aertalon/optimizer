#pragma once

#include <cstddef>
#include <type_traits>

namespace opt ::stdx {

/// https://wg21.link/p1830
template <bool B, class... Deps>
inline constexpr bool dependent_bool_value = B;
template <class... Deps>
inline constexpr bool dependent_false = dependent_bool_value<false, Deps...>;
template <class... Deps>
inline constexpr bool dependent_true = dependent_bool_value<true, Deps...>;

/// https://wg21.link/p2098
template <class T, template <class...> class Primary>
struct is_specialization_of : std::false_type {};
template <template <class...> class Primary, class... Args>
struct is_specialization_of<Primary<Args...>, Primary> : std::true_type {};

template <class T, template <class...> class Primary>
inline constexpr bool is_specialization_of_v =
    is_specialization_of<T, Primary>::value;

template <std::size_t>
struct priority_tag_t;
template <>
struct priority_tag_t<0> {};
template <std::size_t N>
struct priority_tag_t : priority_tag_t<N - 1> {};

template <std::size_t N>
inline constexpr priority_tag_t<N> priority_tag{};

}  // namespace opt::stdx
