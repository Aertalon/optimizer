#include "gcem.hpp"
#include "src/math.hpp"

#include "boost/ut.hpp"

// NOLINTBEGIN(readability-magic-numbers)

auto main() -> int
{
    using namespace boost::ut;
    using opt::dual;

    constexpr dual x{1.0F, 2.0F};

    test("dualnumbers math exp") = [&x] {
        expect(eq(gcem::exp(1.0F), opt::exp(1.0F)));

        expect(eq(opt::exp(x), dual{gcem::exp(1.0F), 2.0F * gcem::exp(1.0F)}));
        expect(
            eq(opt::exp(x * x), dual{gcem::exp(1.0F), 4.0F * gcem::exp(1.0F)}));
    };

    test("dualnumbers math cos") = [&x] {
        expect(eq(gcem::cos(1.0F), opt::cos(1.0F)));

        expect(eq(opt::cos(x), dual{gcem::cos(1.0F), -2.0F * gcem::sin(1.0F)}));
        expect(eq(opt::cos(x * x),
                  dual{gcem::cos(1.0F), -4.0F * gcem::sin(1.0F)}));
    };

    test("dualnumbers math sin") = [&x] {
        expect(eq(gcem::sin(1.0F), opt::sin(1.0F)));

        expect(eq(opt::sin(x), dual{gcem::sin(1.0F), 2.0F * gcem::cos(1.0F)}));
        expect(
            eq(opt::sin(x * x), dual{gcem::sin(1.0F), 4.0F * gcem::cos(1.0F)}));
    };
}

// NOLINTEND(readability-magic-numbers)
