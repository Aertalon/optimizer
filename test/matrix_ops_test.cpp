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
}

// NOLINTEND(readability-magic-numbers)
