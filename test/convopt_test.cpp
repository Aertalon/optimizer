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

        return opt::exp((x[0] - T{3}) * (x[0] - T{3})) +
               (x[1] + T{1}) * (x[1] + T{1});
    };

    constexpr opt::EqualityConstraint eq_constraints{
        .A = opt::matrix<float, 1, 2>{{1.0F}, {0.0F}},
        .b = opt::matrix<float, 1, 1>{opt::from_rows{}, {0.0F}}};

    constexpr auto cost_quadratic = []<opt::Point P>(const P& x) {
        using T = opt::scalar_t<P>;
        return (x[0] - T{3} * x[1]) * (x[0] - T{3} * x[1]) / T{2};
    };

    constexpr point p{2.0F, 0.0F};

    test("convopt gradient") = [&] {
        expect(constant<eq(opt::gradient(p, cost),
                           vector{-2.0F * opt::exp(1.0F), 2.0F})>);
    };

    test("convopt hessian") = [&] {
        constexpr auto H{opt::hessian(p, cost_quadratic)};

        expect(constant<eq(H[0], vector{1.0F, -3.0F})>);
        expect(constant<eq(H[1], vector{-3.0F, 9.0F})>);
    };

    const auto q{p};
    test("convopt gradient") = [&] {
        expect(
            eq(opt::gradient(q, cost), vector{-2.0F * opt::exp(1.0F), 2.0F}));
    };

    test("convopt optimize") = [&] {
        expect(constant<opt::close_to(
                   opt::optimize(p, cost), point{3.0F, -1.0F}, 1e-2F)>);
    };

    test("convopt optimize with equality constraints") = [&] {
        std::cout << opt::optimize(p, cost, eq_constraints) << std::endl;
        // expect(constant<opt::close_to(
        //            opt::optimize(p, cost, eq), point{3.0F, -1.0F}, 1e-2F)>);
    };
}

// Add tests

// NOLINTEND(readability-magic-numbers)
