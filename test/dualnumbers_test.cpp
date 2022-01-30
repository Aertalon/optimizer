#include "src/dualnumbers.hpp"

#include "boost/ut.hpp"

// NOLINTBEGIN(readability-magic-numbers)

auto main() -> int
{
    using namespace boost::ut;
    using opt::dual;

    test("dualnumbers sum") = [] {
        constexpr dual x{1.0F, 0.0F, 0.0F, 0.0F};

        constexpr dual y1{0.0F, 1.0F, 0.0F, 0.0F};
        expect(constant<eq(x + y1, dual{1.0F, 1.0F, 0.0F, 0.0F})>);
        expect(constant<eq(y1 + x, dual{1.0F, 1.0F, 0.0F, 0.0F})>);

        constexpr dual y2{0.0F, 0.0F, 1.0F, 0.0F};
        expect(constant<eq(x + y2, dual{1.0F, 0.0F, 1.0F, 0.0F})>);
        expect(constant<eq(y2 + x, dual{1.0F, 0.0F, 1.0F, 0.0F})>);

        constexpr dual y3{0.0F, 0.0F, 0.0F, 1.0F};
        expect(constant<eq(x + y3, dual{1.0F, 0.0F, 0.0F, 1.0F})>);
        expect(constant<eq(y3 + x, dual{1.0F, 0.0F, 0.0F, 1.0F})>);
    };

    test("dualnumbers negate") = [] {
        constexpr dual x{1.0F, -42.0F, 10.0F, 20.0F};

        expect(constant<eq(-x, dual{-1.0F, 42.0F, -10.0F, -20.0F})>);
        // NOLINTNEXTLINE(misc-redundant-expression)
        expect(constant<eq(x + (-x), x - x)>);
        expect(constant<eq(x + (-x), dual<float>{})>);
    };

    test("dualnumbers product") = [] {
        constexpr dual x{1.0F, 0.0F, 0.0F, 0.0F};
        constexpr dual y{0.0F, 1.0F, 0.0F, 0.0F};

        expect(constant<eq(x * y, dual{0.0F, 1.0F, 0.0F, 0.0F})>);
        expect(constant<eq(y * x, dual{0.0F, 1.0F, 0.0F, 0.0F})>);
    };

    test("dualnumbers division") = [] {
        constexpr dual x{1.0F, 0.0F, 0.0F, 0.0F};
        constexpr dual y{1.0F, 1.0F, 2.0F, 3.0F};
        constexpr dual ratio{x / y};

        expect(constant<eq(x, ratio * y)>);
    };

    // Add tests for / and affine and nonlinear functions
}

// NOLINTEND(readability-magic-numbers)
