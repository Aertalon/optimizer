#pragma once

#include "matrix.hpp"
#include "norms.hpp"

#include <iostream>

namespace opt {

// FIXME: handle this properly
template <class T,
          std::size_t Rows,
          std::size_t Cols,
          std::size_t RowsOther,
          std::size_t ColsOther>
constexpr auto diagonal(matrix<T, Rows, Cols> const& m0,
                        matrix<T, RowsOther, ColsOther> const& m1)
    -> matrix<T, Rows + RowsOther, Cols + ColsOther>
{
    matrix<T, Rows + RowsOther, Cols + ColsOther> diagm{};
    std::transform(
        m0.begin(),
        m0.end(),
        diagm.begin(),
        diagm.begin(),
        [](vector<T, Rows> const& m0_col,
           vector<T, Rows + RowsOther>& diagm_col) {
            std::copy(m0_col.begin(), m0_col.end(), diagm_col.begin());
            return diagm_col;
        });

    std::transform(
        m1.begin(),
        m1.end(),
        diagm.begin() + Cols,
        diagm.begin() + Cols,
        [](vector<T, RowsOther> const& m1_col,
           vector<T, Rows + RowsOther>& diagm_col) {
            std::copy(m1_col.begin(), m1_col.end(), diagm_col.begin() + Rows);
            return diagm_col;
        });

    return diagm;
}

// FIXME: handle this properly
template <std::size_t RowIndex,
          std::size_t ColIndex,
          class T,
          std::size_t Rows,
          std::size_t Cols>
requires(RowIndex < Rows && ColIndex < Cols) constexpr auto submatrix(
    matrix<T, Rows, Cols> const& m)
    -> matrix<T, Rows - RowIndex, Cols - ColIndex>
{
    matrix<T, Rows - RowIndex, Cols - ColIndex> subm{};
    std::transform(
        m.begin() + ColIndex,
        m.end(),
        subm.begin(),
        [](vector<T, Rows> const& m_col) {
            return subvector<RowIndex>(m_col);
        });
    return subm;
}

template <class T, std::size_t Rows, std::size_t Cols>
struct QRFactorization {
    matrix<T, Rows, Rows> Q;
    matrix<T, Rows, Cols> R;
};

/// Compute Householder's transformation for a vector v over coordinate I
/// @tparam The coordinate over which Householder's reflection happens.
/// @tparam T Entry types of vector v.
/// @tparam N Dimension of vector v.
/// @param v The vector upon which Householder's reflection is applied.
template <std::size_t I, class T, std::size_t N>
constexpr auto householder_transformation(vector<T, N> const& v)
    -> matrix<T, N, N>
{

    const auto projection{
        normalize(v - norm<L2>(v) * canonical_vector<T, N, I>)};
    return identity<T, N> -
           matrix<T, N, 1>(projection) * matrix<T, 1, N>(projection) -
           matrix<T, N, 1>(projection) * matrix<T, 1, N>(projection);
}

namespace detail {

template <class T, std::size_t Rows, std::size_t Cols, std::size_t... Is>
requires(Rows >= Cols) constexpr auto compute_QR_impl(
    matrix<T, Rows, Cols> const& m, std::index_sequence<Is...>)
    -> QRFactorization<T, Rows, Cols>
{
    auto r{m};
    auto full_q{identity<T, Rows>};

    auto const handle_column = [&r, &full_q]<std::size_t i>() {
        const auto r_sub{submatrix<i, i>(r)};
        const auto first_column{*r_sub.begin()};
        const auto q_sub{householder_transformation<0>(first_column)};
        const matrix<T, Rows, Rows> q{diagonal(identity<T, i>, q_sub)};
        r = q * r;
        full_q = full_q * q.transpose();
    };

    (handle_column.template operator()<Is>(), ...);

    return {full_q, r};
}
}  // namespace detail

/// Compute QR factorization by means of Householder's reflections
///
/// TODO replace with https://math.berkeley.edu/~mgu/MA273/Strong_RRQR.pdf
/// for higher num stabiity and rank revealing
template <class T, std::size_t Rows, std::size_t Cols>
requires(Rows >= Cols) constexpr auto compute_QR(matrix<T, Rows, Cols> const& m)
    -> QRFactorization<T, Rows, Cols>
{
    return detail::compute_QR_impl(m, std::make_index_sequence<Cols>{});
}

/// Solve the equation Rx = b with R square upper triangular invertible
///
/// TODO: make this into a class
/// TODO: matrices could have "tags", e.g., "UpperTriangular"
/// TODO: add requirement UpperTriangular and Invertible (=diagonal elements !=
/// 0)
template <class T, std::size_t Rows, std::size_t Cols>
requires(Rows == Cols) constexpr auto solve_upper_diagonal(
    matrix<T, Rows, Cols> const& R, matrix<T, Rows, 1> const& b)
    -> matrix<T, Rows, 1>
{
    matrix<T, Rows, 1> x{};
    // Make this nice
    for (std::size_t rr{0}; rr < Rows; ++rr) {
        const auto r{Rows - rr - 1};
        auto rhs{b[{r, 0}]};
        for (std::size_t c{r + 1}; c < Rows; c++) {
            rhs -= R[{r, c}] * x[{c, 0}];
        }
        x[{r, 0}] = rhs / R[{r, r}];
    }
    return x;
}

}  // namespace opt