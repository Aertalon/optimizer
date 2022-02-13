#include "src/spaces.hpp"
#include "src/spaces_ops.hpp"

#include "boost/ut.hpp"

#include <type_traits>

// NOLINTBEGIN(readability-magic-numbers)

auto main() -> int
{
    using namespace boost::ut;

    using opt::subvector;
    using opt::vector;

    using vec = vector<float, 3>;

    test("spaces canonical vector") = [] {
        constexpr auto v0{opt::canonical_vector<vec, 0>};
        expect(constant<eq(v0, vec{1.0F, 0.0F, 0.0F})>);

        constexpr auto v1{opt::canonical_vector<vec, 1>};
        expect(constant<eq(v1, vec{0.0F, 1.0F, 0.0F})>);

        constexpr auto v2{opt::canonical_vector<vec, 2>};
        expect(constant<eq(v2, vec{0.0F, 0.0F, 1.0F})>);
    };

    test("spaces subvector") = [] {
        constexpr vec v0{1.0F, 2.0F, 0.0F};

        // Identity
        constexpr auto actual{subvector<0, 1, 2>(v0)};
        expect(constant<eq(actual, v0)>);

        // Two out of three
        expect(constant<eq(subvector<0, 1>(v0), vector<float, 2>{1.0F, 2.0F})>);
        expect(constant<eq(subvector<1, 2>(v0), vector<float, 2>{2.0F, 0.0F})>);
        expect(constant<eq(subvector<0, 2>(v0), vector<float, 2>{1.0F, 0.0F})>);

        // One
        expect(constant<eq(subvector<0>(v0), vector<float, 1>{1.0F})>);
        expect(constant<eq(subvector<1>(v0), vector<float, 1>{2.0F})>);
        expect(constant<eq(subvector<2>(v0), vector<float, 1>{0.0F})>);

        // Any order
        expect(constant<eq(subvector<1, 2, 0>(v0), vec{2.0F, 0.0F, 1.0F})>);
    };
}

// NOLINTEND(readability-magic-numbers)
