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
requires(matrix<V, Cols>::is_square)
    [[nodiscard]] constexpr auto trace(matrix<V, Cols> const& m) -> scalar_t<V>
{
    return [&m]<std::size_t... Is>(std::index_sequence<Is...>)
    {
        return (m.template get<Is, Is>() + ...);
    }
    (std::make_index_sequence<Cols>{});
}

template <Vector V, std::size_t Cols, Vector W, std::size_t ColsOther>
requires(std::is_same<typename extend_to<V, 1>::type,
                      typename extend_to<W, 1>::type>::value&&
             std::is_same<scalar_t<V>, scalar_t<W>>::
                 value) constexpr auto diagonal(matrix<V, Cols> const& m0,
                                                matrix<W, ColsOther> const& m1)
{
    using U = typename extend_to<V, V::size + W::size>::type;
    matrix<U, Cols + ColsOther> diagm{};
    std::transform(
        m0.begin(),
        m0.end(),
        diagm.begin(),
        diagm.begin(),
        [](V const& m0_col, U& diagm_col) {
            std::copy(m0_col.begin(), m0_col.end(), diagm_col.begin());
            return diagm_col;
        });

    std::transform(
        m1.begin(),
        m1.end(),
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