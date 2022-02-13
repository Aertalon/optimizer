#include "concepts.hpp"

#pragma once

namespace opt {
namespace detail {
template <Vector V, std::size_t I>
    requires(I < V::size)
constexpr auto make_canonical_vector() -> V
{
    using T = scalar_t<V>;
    const auto impl = []<std::size_t... Is>(std::index_sequence<Is...>)
    {
        V v{};
        v.template get<I>() = T{1};
        return v;
    };
    return impl(std::make_index_sequence<V::size>{});
}
}  // namespace detail

template <Vector V, std::size_t I>
    requires(I < V::size)
inline constexpr auto canonical_vector = detail::make_canonical_vector<V, I>();

}  // namespace opt