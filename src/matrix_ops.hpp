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

// FIXME: this should be a matrix method
template <class V, std::size_t Cols>
requires(matrix<V, Cols>::is_square)
    [[nodiscard]] constexpr auto trace(matrix<V, Cols> const& m) -> scalar_t<V>
{
    return [&m]<std::size_t... Is>(std::index_sequence<Is...>)
    {
        return (m.template get<Is, Is>() + ...);
    }
    (std::make_index_sequence<Cols>{});
}

}  // namespace opt