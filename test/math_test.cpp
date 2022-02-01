#include "src/math.hpp"
#include "src/spaces.hpp"

#include "boost/ut.hpp"

#include <cmath>

// NOLINTBEGIN(readability-magic-numbers)

auto main() -> int
{
    using namespace boost::ut;
    using opt::dual;

    constexpr dual x{1.0F, 2.0F, 3.0F, 0.0F};
    constexpr float tol{1e-5F};

    test("dualnumbers math square") = [&x] {
        expect(constant<eq(x * x,
                           dual{1.0F,
                                2.0F * 2.0F * 1.0F,
                                3.0F * 2.0F * 1.0F,
                                2.0F * 3.0F * 2.0F})>);
    };

    test("dualnumbers math exp") = [&x, tol] {
        expect(le(std::exp(1.0F), opt::exp(1.0F) + tol) and
               ge(std::exp(1.0F), opt::exp(1.0F) - tol));

        expect(constant<opt::close_to(opt::exp(x),
                                      dual{opt::exp(1.0F),
                                           2.0F * opt::exp(1.0F),
                                           3.0F * opt::exp(1.0F),
                                           6.0F * opt::exp(1.0F)},
                                      tol)>);

        constexpr dual y{1.0F, 1.0F, 1.0F, 0.0F};
        expect(constant<opt::close_to(
                   opt::exp(y * y),
                   dual{
                       opt::exp(1.0F),
                       2.0F * opt::exp(1.0F),
                       2.0F * opt::exp(1.0F),
                       (2.0F + 4.0F) * opt::exp(1.0F),
                   },
                   tol)>);
    };

    test("dualnumbers math cos") = [&x, tol] {
        expect(eq(std::cos(1.0F), opt::cos(1.0F)));

        expect(opt::close_to(
            opt::cos(x),
            dual{std::cos(1.0F),
                 -2.0F * std::sin(1.0F),
                 -3.0F * std::sin(1.0F),
                 -6.0F * std::cos(1.0F)},
            tol));
        expect(opt::close_to(
            opt::cos(x * x),
            dual{
                std::cos(1.0F),
                2.0F * (-2.0F * std::sin(1.0F)),
                3.0F * (-2.0F * std::sin(1.0F)),
                2.0F * 3.0F * (-2.0F * std::sin(1.0F) - 4.0F * std::cos(1.0F))},
            tol));
    };

    test("dualnumbers math sin") = [&x, tol] {
        expect(eq(std::sin(1.0F), opt::sin(1.0F)));

        expect(opt::close_to(
            opt::sin(x),
            dual{std::sin(1.0F),
                 2.0F * std::cos(1.0F),
                 3.0F * std::cos(1.0F),
                 -6.0F * std::sin(1.0F)},
            tol));
        expect(opt::close_to(
            opt::sin(x * x),
            dual{std::sin(1.0F),
                 2.0F * (2.0F * std::cos(1.0F)),
                 3.0F * (2.0F * std::cos(1.0F)),
                 2.0F * 3.0F * (2.0F * std::cos(1.0F) - 4.0F * std::sin(1.0F))},
            tol));
    };

    test("math sqrt") = [tol] {
        const float result{opt::sqrt(2.0F)};
        expect(le(result * result, 2.0F + tol) and
               ge(result * result, 2.0F - tol));
    };
}

// NOLINTEND(readability-magic-numbers)
