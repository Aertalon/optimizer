#include "src/math.hpp"

#include "boost/ut.hpp"

#include <cmath>

// NOLINTBEGIN(readability-magic-numbers)

auto main() -> int
{
    using namespace boost::ut;
    using opt::dual;

    constexpr dual x{1.0F, 2.0F};

    test("dualnumbers math exp") = [&x] {
        expect(eq(std::exp(1.0F), opt::exp(1.0F)));

        expect(eq(opt::exp(x), dual{std::exp(1.0F), 2.0F * std::exp(1.0F)}));
        expect(
            eq(opt::exp(x * x), dual{std::exp(1.0F), 4.0F * std::exp(1.0F)}));
    };

    test("dualnumbers math cos") = [&x] {
        expect(eq(std::cos(1.0F), opt::cos(1.0F)));

        expect(eq(opt::cos(x), dual{std::cos(1.0F), -2.0F * std::sin(1.0F)}));
        expect(
            eq(opt::cos(x * x), dual{std::cos(1.0F), -4.0F * std::sin(1.0F)}));
    };

    test("dualnumbers math sin") = [&x] {
        expect(eq(std::sin(1.0F), opt::sin(1.0F)));

        expect(eq(opt::sin(x), dual{std::sin(1.0F), 2.0F * std::cos(1.0F)}));
        expect(
            eq(opt::sin(x * x), dual{std::sin(1.0F), 4.0F * std::cos(1.0F)}));
    };
}

// NOLINTEND(readability-magic-numbers)
