#include "src/norms.hpp"
#include "src/spaces.hpp"

#include "boost/ut.hpp"

#include <type_traits>

// NOLINTBEGIN(readability-magic-numbers)

auto main() -> int
{
    using namespace boost::ut;

    using opt::point;
    using opt::vector;

    test("spaces vector") = [] {
        constexpr vector<float, 3> zero{};
        constexpr vector v1{1.0F, 0.0F, 0.0F};
        constexpr vector v2{1.0F, 0.0F, -1.0F};

        expect(constant<v1[0] == 1._f>);
        expect(constant<v1[1] == 0._f>);
        expect(constant<v1[2] == 0._f>);

        expect(constant<eq(v1 + zero, v1)>);

        expect(constant<eq(-v1, vector{-1.0F, 0.0F, 0.0F})>);
        expect(constant<eq(v1 + v2, vector{2.0F, 0.0F, -1.0F})>);

        expect(constant<eq(3.0F * v2, vector{3.0F, 0.0F, -3.0F})>);
        expect(constant<eq(v2 * 3.0F, vector{3.0F, 0.0F, -3.0F})>);
    };

    test("spaces vector not slicable") = [] {
        expect(constant<
               not std::is_constructible_v<opt::entity<vector<float, 3>>>>);
        expect(
            constant<not std::is_constructible_v<opt::entity<vector<float, 3>>,
                                                 vector<float, 3>>>);
    };

    test("spaces vector and point sum") = [] {
        constexpr point<float, 3> zero{};
        constexpr point p1{1.0F, 0.0F, 0.0F};
        constexpr vector v{1.0F, 0.0F, 0.0F};

        expect(constant<eq(zero + v, p1)>);
        expect(constant<eq(point<float, 3>{} + vector{1.0F, 0.0F, 0.0F}, p1)>);
    };

    test("spaces canonical vector") = [] {
        constexpr auto actual{opt::canonical_vector<float, 3, 1>};
        constexpr vector<float, 3> expected{0.0F, 1.0F, 0.0F};

        expect(constant<eq(actual, expected)>);
    };
}

// NOLINTEND(readability-magic-numbers)
