#include "src/matrix_factorizations.hpp"
#include "src/matrix_ops.hpp"

#include "boost/ut.hpp"

// NOLINTBEGIN(readability-magic-numbers)

namespace {
using opt::matrix;
using opt::vector;

using vec = vector<float, 3>;

template <class T, std::size_t Rows, std::size_t Cols>
using mat = matrix<vector<T, Rows>, Cols>;
}  // namespace

auto main() -> int
{
    using namespace boost::ut;

    test("matrix identity") = [] {
        // clang-format off
        constexpr mat<float, 3, 3> expected{
            1.0F, 0.0F, 0.0F,
            0.0F, 1.0F, 0.0F,
            0.0F, 0.0F, 1.0F};
        // clang-format on
        constexpr vec v{2.0F, -4.0F, 42.F};

        expect(constant<eq(expected, opt::identity<vec>)>);
        expect(constant<eq(v, opt::identity<vec> * v)>);
    };

    test("matrix trace") = [] {
        // clang-format off
        constexpr mat<float, 3, 3> m{
            1.0F, -42.0F, -42.0F,
            -42.0F, 2.0F, -42.0F,
            -42.0F, -42.0F, 3.0F};
        // clang-format on
        expect(constant<eq(trace(m), 6.0F)>);
    };

    test("matrix diagonal") = [] {
        constexpr mat<float, 1, 1> m0{2.0F};
        // clang-format off
        constexpr mat<float, 3, 2> m1{
            1.0F, 2.0F,
            -1.0F, -2.0F,
            0.0F, 0.0F};
        constexpr auto actual_diag{diagonal(m0, m1)};

        constexpr mat<float, 4, 3> expected_diag{
            2.0F, 0.0F, 0.0F,
            0.0F, 1.0F, 2.0F,
            0.0F, -1.0F, -2.0F,
            0.0F, 0.0F, 0.0F};
        // clang-format on
        expect(constant<eq(actual_diag, expected_diag)>);
    };

    test("matrix submatrix") = [] {
        using mat23 = mat<float, 2, 3>;
        constexpr mat23 m{
            // clang-format off
            1.0F, 2.0F, 0.0F,
            -1.0F, -2.0F, 0.0F};
        // clang-format on

        using selected_rows = mat23::row_sequence<0U>;
        using selected_cols = mat23::col_sequence<0U, 2U>;

        constexpr auto m0{opt::submatrix<selected_rows, selected_cols>(m)};
        constexpr mat<float, 1, 2> expected_m0{1.0F, 0.0F};
        expect(constant<eq(m0, expected_m0)>);
        expect(constant<eq(m0, mat<float, 1, 2>{1.0F, 0.0F})>);
    };

    test("matrix lower submatrix") = [] {
        using mat23 = mat<float, 2, 3>;
        constexpr mat23 m{
            // clang-format off
            1.0F, 2.0F, 0.0F,
            -1.0F, -2.0F, 0.0F};
        // clang-format on

        using selected_rows =
            opt::stdx::bounded_integer_sequence<mat23::row_index_type, 1U, 2U>;
        using selected_cols =
            opt::stdx::bounded_integer_sequence<mat23::col_index_type, 1U, 3U>;

        constexpr auto m0{opt::submatrix<selected_rows, selected_cols>(m)};
        constexpr mat<float, 1, 2> expected_m0{-2.0F, 0.0F};
        expect(constant<eq(m0, expected_m0)>);
        expect(constant<eq(m0, mat<float, 1, 2>{-2.0F, 0.0F})>);
    };

    test("matrix qr factorization square") = [] {
        constexpr mat<float, 3, 3> m{
            // clang-format off
            12.0F, -51.0F, 4.0F,
            6.0F, 167.0F, -68.0F,
            -4.0F, 24.0F, -41.0F};
        // clang-format on

        constexpr auto qr{opt::compute_QR(m)};

        constexpr auto check_on_q{
            (qr.Q) * (qr.Q).transpose() - opt::identity<vec>};
        constexpr auto check_on_q_and_r{(qr.Q) * (qr.R) - m};

        // FIXME: these numerical bounds are bad. Improve!
        expect(constant<le(opt::norm<opt::Frobenius>(check_on_q), 1e-4F)>);
        expect(
            constant<le(opt::norm<opt::Frobenius>(check_on_q_and_r), 1e-3F)>);
    };

    test("matrix qr factorization rectangular") = [] {
        constexpr mat<float, 4, 3> m{
            // clang-format off
            12.0F, -51.0F, 4.0F,
            6.0F, 167.0F, -68.0F,
            -4.0F, 24.0F, -41.0F,
            -1.0F, 2.0F, 1.0F};
        // clang-format on

        constexpr auto qr{opt::compute_QR(m)};

        constexpr auto check_on_q{
            (qr.Q) * (qr.Q).transpose() - opt::identity<vector<float, 4>>};
        constexpr auto check_on_q_and_r{(qr.Q) * (qr.R) - m};

        // FIXME: these numerical bounds are bad. Improve!
        expect(constant<le(opt::norm<opt::Frobenius>(check_on_q), 1e-4F)>);
        expect(
            constant<le(opt::norm<opt::Frobenius>(check_on_q_and_r), 1e-3F)>);
    };
}

// NOLINTEND(readability-magic-numbers)
