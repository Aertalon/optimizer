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
}

// NOLINTEND(readability-magic-numbers)
