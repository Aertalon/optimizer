#include "src/lineq_solvers.hpp"
#include "src/matrix.hpp"
#include "src/spaces.hpp"

#include "boost/ut.hpp"

// NOLINTBEGIN(readability-magic-numbers)

namespace {
using opt::matrix;
using opt::solve_upper_diagonal;
using opt::vector;

template <class T, std::size_t Rows, std::size_t Cols>
using mat = matrix<vector<T, Rows>, Cols>;
}  // namespace

auto main() -> int
{
    using namespace boost::ut;

    test("solve Rx = b, R upper triangular, b vector") = [] {
        // clang-format off
        constexpr mat<float, 2, 2> R{
            1.0F, 2.0F,
            0.0F, -1.0F};
        // clang-format on
        constexpr vector<float, 2> b{1.0F, 1.0F};
        constexpr vector<float, 2> expected{3.0F, -1.0F};

        constexpr auto actual{solve_upper_diagonal(R, b)};
        expect(constant<eq(actual, expected)>);
    };
}

// NOLINTEND(readability-magic-numbers)
