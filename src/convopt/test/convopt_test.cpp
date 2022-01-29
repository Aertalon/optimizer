#include "src/convopt/convopt.hpp"
#include "src/dualnumbers/math.hpp"
#include "src/spaces/spaces.hpp"

#include "boost/ut.hpp"

// NOLINTBEGIN(readability-magic-numbers)

auto main() -> int
{
    using namespace boost::ut;
    using point = opt::Point<float, 2>;
    using vec = opt::Vector<float, 2>;

    constexpr auto cost =
        []<class T, std::size_t N, template <class, std::size_t> class P>(
            const P<T, N>& x) {
            auto x_part = x[0] - T{3.0F};
            auto y_part = x[1] - T{-1.0F};

            return opt::exp(x_part * x_part) + y_part * y_part;
        };

    constexpr point p{1.0F, 0.0F};

    test("convopt gradient") = [&] {
        constexpr auto expr =
            eq(opt::gradient(p, cost), vec{-4.0F * opt::exp(4.0F), 2.0F});
        expect(expr);
    };

    test("convopt optimize") = [&] {
        constexpr auto expr =
            close_to(opt::optimize(p, cost), point{3.0F, -1.0F}, 1e-3F);
        expect(expr);
    };
}

// Add tests

// NOLINTEND(readability-magic-numbers)
