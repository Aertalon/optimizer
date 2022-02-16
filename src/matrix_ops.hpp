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

template <Vector V, std::size_t Cols, Vector W, std::size_t ColsOther>
// clang-format off
requires(std::is_same<scalar_t<V>, scalar_t<W>>::value)
// clang-format off
constexpr auto diagonal(matrix<V, Cols> const& m0, matrix<W, ColsOther> const& m1)
{
    using U = reshaped_to_t<V, V::size + W::size>;
    matrix<U, Cols + ColsOther> diagm{};
    std::transform(
        m0.cbegin(),
        m0.cend(),
        diagm.begin(),
        diagm.begin(),
        [](V const& m0_col, U& diagm_col) {
            std::copy(m0_col.begin(), m0_col.end(), diagm_col.begin());
            return diagm_col;
        });

    std::transform(
        m1.cbegin(),
        m1.cend(),
        std::next(diagm.begin(), Cols),
        std::next(diagm.begin(), Cols),
        [](W const& m1_col, U& diagm_col) {
            std::copy(m1_col.begin(),
                      m1_col.end(),
                      std::next(diagm_col.begin(), V::size));
            return diagm_col;
        });

    return diagm;
}

}  // namespace opt