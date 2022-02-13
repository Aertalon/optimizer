#pragma once

#include <cstddef>
#include <type_traits>
#include <utility>

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

template <class T, class U>
struct specialize_same_template : std::is_same<T, U> {};
template <template <class...> typename Base, class... Ts, class... Us>
struct specialize_same_template<Base<Ts...>, Base<Us...>> : std::true_type {};

template <class T, T, class>
struct offset_sequence {};

template <class T, T Offset, T... Values>
struct offset_sequence<T, Offset, std::integer_sequence<T, Values...>> {
    using type = std::integer_sequence<T, Offset + Values...>;
};

template <class T, T Low, T High>
    requires(std::is_integral_v<T>)
using bounded_integer_sequence =
    typename offset_sequence<T,
                             Low,
                             std::make_integer_sequence<T, High - Low>>::type;

}  // namespace opt::stdx
