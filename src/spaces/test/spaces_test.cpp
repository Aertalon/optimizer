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

        static_assert(v1.get<0>() == 1.0F);
        static_assert(v1.get<1>() == 0.0F);
        static_assert(v1.get<2>() == 0.0F);

        static_assert(v1 + zero == v1);

        static_assert(-v1 == vec{-1.0F, 0.0F, 0.0F});
        static_assert(v1 + v2 == vec{2.0F, 0.0F, -1.0F});

        static_assert(3.0F * v2 == vec{3.0F, 0.0F, -3.0F});
        static_assert(v2 * 3.0F == vec{3.0F, 0.0F, -3.0F});
    };

    test("spaces vector norm") = [] {
        constexpr vec v{2.0F, 0.0F, -1.0F};
        static_assert(norm(v) == 5.0F);
    };

    test("spaces vector norm") = [] {
        constexpr point zero{};
        constexpr point p1{1.0F, 0.0F, 0.0F};
        constexpr vec v{1.0F, 0.0F, 0.0F};

        static_assert(zero + v == p1);
        static_assert(point{} + vec{1.0F, 0.0F, 0.0F} == p1);
    };
}

// NOLINTEND(readability-magic-numbers)
