#pragma once

#include "spaces.hpp"

namespace opt {

struct L2 {};

namespace detail {

template <class NormType = L2>
struct norm_fn {

    template <class V>
    requires Vector<V>
    [[nodiscard]] constexpr auto operator()(V const& v) const -> scalar_t<V>
    {
        return sqrt([&v]<std::size_t... Is>(std::index_sequence<Is...>) {
            return ((v[Is] * v[Is]) + ...);
        }(std::make_index_sequence<std::tuple_size_v<V>>{}));
    }
};
}  // namespace detail

template <class NormType>
inline constexpr auto norm = detail::norm_fn<NormType>{};

template <class T>
requires Vector<T>
[[nodiscard]] constexpr auto normalize(const T& v) -> T
{
    return v * (scalar_t<T>{1} / norm<L2>(v));
}

}  // namespace opt