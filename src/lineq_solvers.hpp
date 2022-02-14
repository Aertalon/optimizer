#pragma once

#include "matrix.hpp"
#include "spaces.hpp"

namespace opt {

/// Solve the equation Rx = b with R square upper triangular invertible
///
/// @pre R is upper triangular and invertible
/// TODO: matrices could have "tags", e.g., "UpperTriangular"
/// TODO: add requirement UpperTriangular and Invertible (=diagonal elements !=
/// 0)
template <Vector V, std::size_t Cols>
    requires(matrix<V, Cols>::is_square)
constexpr auto solve_upper_diagonal(matrix<V, Cols> const& R, V const& b) -> V
{
    using M = matrix<V, Cols>;

    V x{};
    [&x, &R, &b]<typename M::row_index_type... rs>(
        typename M::template row_sequence<rs...>) {
        (
            [&x, &R, &b]<typename M::row_index_type rr,
                         typename M::col_index_type... cs>(
                typename M::template col_sequence<cs...>,
                typename M::template row_sequence<rr>) {
                constexpr auto r{M::rows - rr - 1};

                // Solving the r-th row, assuming x(r+1), ..., is known:
                // x(r) = (b(r) - SUM(R(r, c) * x(c))) / R(r, r)
                auto rhs{b.template get<r>()};
                ((rhs -= R.template get<r, cs>() * x.template get<cs>()), ...);
                x.template get<r>() = rhs / (R.template get<r, r>());
            }(opt::stdx::bounded_integer_sequence<
                  typename M::col_index_type,
                  typename M::col_index_type{M::rows - rs},
                  M::cols>{},
              typename M::template row_sequence<rs>{}),
            ...);
    }(make_sequence_of<typename M::row_index_type, M::rows>());

    return x;
}

}  // namespace opt