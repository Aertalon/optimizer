#include "src/spaces/spaces.hpp"

#include "boost/ut.hpp"

// NOLINTBEGIN(readability-magic-numbers)

auto main() -> int
{
    using namespace boost::ut;

    using vec = opt::Vector<float, 3>;
    using point = opt::Point<float, 3>;

    test("spaces vector") = [] {
        constexpr vec zero{};
        constexpr vec v1{1.0F, 0.0F, 0.0F};
        constexpr vec v2{1.0F, 0.0F, -1.0F};

        expect(constant<v1[0] == 1._f>);
        expect(constant<v1[1] == 0._f>);
        expect(constant<v1[2] == 0._f>);

        constexpr auto expr1 = eq(v1 + zero, v1);
        expect(expr1);

        constexpr auto expr2 = eq(-v1, vec{-1.0F, 0.0F, 0.0F});
        expect(expr2);
        constexpr auto expr3 = eq(v1 + v2, vec{2.0F, 0.0F, -1.0F});
        expect(expr3);

        constexpr auto expr4 = eq(3.0F * v2, vec{3.0F, 0.0F, -3.0F});
        expect(expr4);
        constexpr auto expr5 = eq(v2 * 3.0F, vec{3.0F, 0.0F, -3.0F});
        expect(expr5);
    };

    test("spaces vector norm") = [] {
        constexpr vec v{2.0F, 0.0F, -1.0F};

        expect(constant<norm(v) == 5._f>);
    };

    test("spaces vector norm") = [] {
        constexpr point zero{};
        constexpr point p1{1.0F, 0.0F, 0.0F};
        constexpr vec v{1.0F, 0.0F, 0.0F};

        constexpr auto expr1 = eq(zero + v, p1);
        expect(expr1);

        constexpr auto expr2 = eq(point{} + vec{1.0F, 0.0F, 0.0F}, p1);
        expect(expr2);
    };
}

// NOLINTEND(readability-magic-numbers)
