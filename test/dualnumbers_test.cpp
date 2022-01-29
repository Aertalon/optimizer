#include "src/dualnumbers.hpp"

#include "boost/ut.hpp"

// NOLINTBEGIN(readability-magic-numbers)

auto main() -> int
{
    using namespace boost::ut;
    using opt::dual;

    test("dualnumbers sum") = [] {
        constexpr dual x{1.0F, 0.0F};
        constexpr dual y{0.0F, 1.0F};

        expect(constant<eq(x + y, dual{1.0F, 1.0F})>);
        expect(constant<eq(y + x, dual{1.0F, 1.0F})>);
    };

    test("dualnumbers negate") = [] {
        constexpr dual x{1.0F, -42.0F};

        expect(constant<eq(-x, dual{-1.0F, 42.0F})>);
        // NOLINTNEXTLINE(misc-redundant-expression)
        expect(constant<eq(x + (-x), x - x)>);
        expect(constant<eq(x + (-x), dual<float>{})>);
    };

    test("dualnumbers product") = [] {
        constexpr dual x{1.0F, 0.0F};
        constexpr dual y{0.0F, 1.0F};

        expect(constant<eq(x * y, dual{0.0F, 1.0F})>);
        expect(constant<eq(y * x, dual{0.0F, 1.0F})>);
    };

    // Add tests for / and affine and nonlinear functions
}

// NOLINTEND(readability-magic-numbers)
