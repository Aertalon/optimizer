#include "src/convopt.hpp"
#include "src/math.hpp"
#include "src/spaces.hpp"

#include "boost/ut.hpp"

// NOLINTBEGIN(readability-magic-numbers)

auto main() -> int
{
    using namespace boost::ut;
    using opt::point;
    using opt::vector;

    constexpr auto cost = []<opt::Point P>(const P& x) {
        using T = opt::scalar_t<P>;

        auto x_part = x[0] - T{3.0F};
        auto y_part = x[1] - T{-1.0F};

        return opt::exp(x_part * x_part) + y_part * y_part;
    };

    constexpr point p{2.0F, 0.0F};

    test("convopt gradient") = [&] {
        expect(constant<eq(opt::gradient(p, cost),
                           vector{-2.0F * opt::exp(1.0F), 2.0F})>);
    };

    test("convopt optimize") = [&] {
        expect(constant<opt::close_to(
                   opt::optimize(p, cost), point{3.0F, -1.0F}, 1e-3F)>);
    };
}

// Add tests

// NOLINTEND(readability-magic-numbers)
