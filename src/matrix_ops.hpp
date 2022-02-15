#pragma once

#include "matrix.hpp"

namespace opt {

namespace detail {

template <Vector V>
constexpr auto make_identity() -> matrix<V, V::size>
{
    return []<std::size_t... Is>(std::index_sequence<Is...>)
    {
        return matrix<V, V::size>{canonical_vector<V, Is>...};
    }
    (std::make_index_sequence<V::size>{});
}

}  // namespace detail

// Replace with template<Matrix M> (M square) when the concept is available
template <Vector V>
inline constexpr auto identity = detail::make_identity<V>();

template <class V, std::size_t Cols>
// clang-format off
requires(matrix<V, Cols>::is_square)
[[nodiscard]] constexpr auto trace(matrix<V, Cols> const& m) -> scalar_t<V>
// clang-format on
{
    return [&m]<std::size_t... Is>(std::index_sequence<Is...>)
    {
        return (m.template get<Is, Is>() + ...);
    }
    (std::make_index_sequence<Cols>{});
}

}  // namespace opt