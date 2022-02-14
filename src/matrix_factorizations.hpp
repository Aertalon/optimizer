#pragma once

#include "matrix_ops.hpp"
#include "norms.hpp"

namespace opt {

/// Compute Householder's transformation for a vector v over coordinate I
/// @tparam The coordinate over which Householder's reflection happens.
/// @tparam V The vector type.
/// @param v The vector upon which Householder's reflection is applied.
template <std::size_t I, Vector V>
constexpr auto householder_transformation(V const& v) -> matrix<V, V::size>
{
    const auto projection{
        matrix<V, 1>{normalize(v - norm<L2>(v) * canonical_vector<V, I>)}};
    return identity<V> - scalar_t<V>{2} * projection * projection.transpose();
}

/// QRFactorization
/// A struct to hold the decomposition of a matrix into Q and R parts
template <Vector V, std::size_t Cols>
struct QRFactorization {
    matrix<V, V::size> Q;
    matrix<V, Cols> R;
};

/// Compute QR factorization by means of Householder's reflections
///
/// TODO replace with https://math.berkeley.edu/~mgu/MA273/Strong_RRQR.pdf
/// for higher num stabiity and rank revealing
template <Vector V, std::size_t Cols>
    requires(V::size >= Cols)
constexpr auto compute_QR(matrix<V, Cols> const& m) -> QRFactorization<V, Cols>
{
    using M = matrix<V, Cols>;
    using matQ = matrix<V, V::size>;

    return [&m]<std::size_t... Is>(std::integer_sequence<std::size_t, Is...>)
        ->QRFactorization<V, Cols>
    {
        auto r{m};
        auto full_q{identity<V>};

        auto const handle_column = [&r, &full_q]<std::size_t I>() {
            const auto r_sub{submatrix<
                opt::stdx::bounded_integer_sequence<std::size_t, I, M::rows>,
                opt::stdx::bounded_integer_sequence<std::size_t, I, M::cols>>(
                r)};
            const auto first_column{*r_sub.begin()};
            const auto q_sub{householder_transformation<0>(first_column)};
            const matQ q{diagonal(identity<extend_to_t<V, I>>, q_sub)};
            r = q * r;
            full_q = full_q * q.transpose();
        };

        (handle_column.template operator()<Is>(), ...);

        return {full_q, r};
    }
    (std::make_index_sequence<M::cols>{});
}

}  // namespace opt