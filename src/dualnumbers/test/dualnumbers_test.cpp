#include "src/dualnumbers/dualnumbers.hpp"

#include "boost/ut.hpp"

// NOLINTBEGIN(readability-magic-numbers)

auto main() -> int
{
    using namespace boost::ut;
    using opt::DualNumber;

    test("dualnumbers sum") = [] {
        constexpr DualNumber x{1.0F, 0.0F};
        constexpr DualNumber y{0.0F, 1.0F};

        expect(constant<eq(x + y, DualNumber{1.0F, 1.0F})>);
        expect(constant<eq(y + x, DualNumber{1.0F, 1.0F})>);
    };

    test("dualnumbers negate") = [] {
        constexpr DualNumber x{1.0F, -42.0F};

        expect(constant<eq(-x, DualNumber{-1.0F, 42.0F})>);
        // NOLINTNEXTLINE(misc-redundant-expression)
        expect(constant<eq(x + (-x), x - x)>);
        expect(constant<eq(x + (-x), DualNumber{})>);
    };

    test("dualnumbers product") = [] {
        constexpr DualNumber x{1.0F, 0.0F};
        constexpr DualNumber y{0.0F, 1.0F};

        expect(constant<eq(x * y, DualNumber{0.0F, 1.0F})>);
        expect(constant<eq(y * x, DualNumber{0.0F, 1.0F})>);
    };

    // Add tests for / and affine and nonlinear functions
}

// NOLINTEND(readability-magic-numbers)
