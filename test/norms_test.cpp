#include "src/matrix.hpp"
#include "src/norms.hpp"
#include "src/spaces.hpp"

#include "boost/ut.hpp"

#include <type_traits>

// NOLINTBEGIN(readability-magic-numbers)

auto main() -> int
{
    using namespace boost::ut;

    using opt::norm;
    using opt::normalize;
    using opt::vector;

    test("spaces vector norm, normalize") = [] {
        constexpr vector v{4.0F, 0.0F, -3.0F};
        constexpr vector v_normalized{4.0F / 5.0F, 0.0F, -3.0F / 5.0F};

        expect(constant<eq(norm<opt::L2>(v), 5._f)>);
        expect(constant<eq(normalize(v), v_normalized)>);
    };
}

// NOLINTEND(readability-magic-numbers)
