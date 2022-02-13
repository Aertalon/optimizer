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

template <std::size_t... Ns>
struct subvector_fn {

    template <Vector V>
        requires(std::is_convertible_v<typename V::index_type, std::size_t> &&
                 // NOLINTNEXTLINE(misc-redundant-expression)
                 ((Ns < V::size) && ...))
    constexpr auto operator()(V const& v) const -> extend_to_t<V, sizeof...(Ns)>
    {
        using subvector_type = extend_to_t<V, sizeof...(Ns)>;
        return subvector_type{v.template get<Ns>()...};
    }
};

template <std::size_t... Ns>
inline constexpr auto subvector = subvector_fn<Ns...>{};

}  // namespace opt