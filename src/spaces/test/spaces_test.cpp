#include "src/spaces/spaces.hpp"

#include "boost/ut.hpp"

#include <type_traits>

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

        expect(constant<eq(v1 + zero, v1)>);

        expect(constant<eq(-v1, vec{-1.0F, 0.0F, 0.0F})>);
        expect(constant<eq(v1 + v2, vec{2.0F, 0.0F, -1.0F})>);

        expect(constant<eq(3.0F * v2, vec{3.0F, 0.0F, -3.0F})>);
        expect(constant<eq(v2 * 3.0F, vec{3.0F, 0.0F, -3.0F})>);
    };

    test("spaces vector not slicable") = [] {
        expect(constant<not std::is_constructible_v<
                   opt::Entity<opt::Vector<float, 3>>>>);
        expect(constant<
               not std::is_constructible_v<opt::Entity<opt::Vector<float, 3>>,
                                           opt::Vector<float, 3>>>);
    };

    test("spaces vector norm") = [] {
        constexpr vec v{2.0F, 0.0F, -1.0F};

        expect(constant<norm(v) == 5._f>);
    };

    test("spaces vector norm") = [] {
        constexpr point zero{};
        constexpr point p1{1.0F, 0.0F, 0.0F};
        constexpr vec v{1.0F, 0.0F, 0.0F};

        expect(constant<eq(zero + v, p1)>);
        expect(constant<eq(point{} + vec{1.0F, 0.0F, 0.0F}, p1)>);
    };
}

// NOLINTEND(readability-magic-numbers)
