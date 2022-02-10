#include "src/spaces.hpp"
#include "src/spaces_ops.hpp"

#include "boost/ut.hpp"

#include <type_traits>

// NOLINTBEGIN(readability-magic-numbers)

auto main() -> int
{
    using namespace boost::ut;

    using vec = opt::vector<float, 3>;

    test("spaces canonical vector") = [] {
        constexpr auto v0{opt::canonical_vector<vec, 0>};
        expect(constant<eq(v0, vec{1.0F, 0.0F, 0.0F})>);

        constexpr auto v1{opt::canonical_vector<vec, 1>};
        expect(constant<eq(v1, vec{0.0F, 1.0F, 0.0F})>);

        constexpr auto v2{opt::canonical_vector<vec, 2>};
        expect(constant<eq(v2, vec{0.0F, 0.0F, 1.0F})>);
    };
}

// NOLINTEND(readability-magic-numbers)
