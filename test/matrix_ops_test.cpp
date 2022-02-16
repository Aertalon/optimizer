#include "src/matrix_ops.hpp"

#include "boost/ut.hpp"

// NOLINTBEGIN(readability-magic-numbers)

auto main() -> int
{
    using namespace boost::ut;

    using opt::matrix;
    using opt::vector;

    using vec = vector<float, 3>;

    test("matrix identity") = [] {
        // clang-format off
        constexpr matrix<vec, 3> expected{
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
        constexpr matrix<vec, 3> m{
            1.0F, -42.0F, -42.0F,
            -42.0F, 2.0F, -42.0F,
            -42.0F, -42.0F, 3.0F};
        // clang-format on
        expect(constant<eq(trace(m), 6.0F)>);
    };

    test("matrix diagonal") = [] {
        constexpr matrix<vector<float, 1>, 1> m0{2.0F};
        // clang-format off
        constexpr matrix<vector<float, 3>, 2> m1{
            1.0F, 2.0F,
            -1.0F, -2.0F,
            0.0F, 0.0F};
        constexpr auto actual_diag{diagonal(m0, m1)};

        constexpr matrix<vector<float, 4>, 3> expected_diag{
            2.0F, 0.0F, 0.0F,
            0.0F, 1.0F, 2.0F,
            0.0F, -1.0F, -2.0F,
            0.0F, 0.0F, 0.0F};
        // clang-format on
        expect(constant<eq(actual_diag, expected_diag)>);
    };
}

// NOLINTEND(readability-magic-numbers)
