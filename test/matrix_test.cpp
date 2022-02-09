#include "src/matrix.hpp"
#include "src/spaces.hpp"

#include "boost/ut.hpp"

// NOLINTBEGIN(readability-magic-numbers)

auto main() -> int
{
    using namespace boost::ut;

    using opt::matrix;
    using opt::vector;

    test("rectangular matrix ctor") = [] {
        // By column
        constexpr matrix<vector<float, 3>, 2> m_by_column_vectors{
            vector<float, 3>{1.0F, -1.0F, 0.0F},
            vector<float, 3>{2.0F, -2.0F, 0.0F}};

        // By column - init lists
        constexpr matrix<vector<float, 3>, 2> m_by_column_init_lists{
            {1.0F, -1.0F, 0.0F}, {2.0F, -2.0F, 0.0F}};
        expect(constant<eq(m_by_column_vectors, m_by_column_init_lists)>);

        // By row
        // clang-format off
        constexpr matrix<vector<float, 3>, 2> m_by_rows{
            1.0F, 2.0F, -1.0F,
            -2.0F, 0.0F, 0.0F};
        // clang-format on

        expect(constant<eq(m_by_column_vectors, m_by_rows)>);
    };

    test("matrix indices") = [] {
        // clang-format off
        constexpr matrix<vector<float, 2>, 2> m{
            1.0F, 2.0F,
            -2.0F, 0.0F};
        // clang-format on

        expect(constant<eq(m.get<0, 0>(), 1.0F)>);
        expect(eq(m[{0, 0}], 1.0F));

        expect(constant<eq(m.get<1, 0>(), -2.0F)>);
        expect(eq(m[{1, 0}], -2.0F));

        expect(constant<eq(m.get<0, 1>(), 2.0F)>);
        expect(eq(m[{0, 1}], 2.0F));

        expect(constant<eq(m.get<1, 1>(), 0.0F)>);
        expect(eq(m[{1, 1}], 0.0F));
    };
}

// NOLINTEND(readability-magic-numbers)
