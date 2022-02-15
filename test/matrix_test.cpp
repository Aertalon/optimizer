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
        constexpr matrix<vector<float, 3>, 2> m_by_rows{
            1.0F, 2.0F, -1.0F, -2.0F, 0.0F, 0.0F};

        expect(constant<eq(m_by_column_vectors, m_by_rows)>);
    };

    test("square matrix ctor") = [] {
        // By column - init lists
        constexpr matrix<vector<float, 2>, 2> m_by_column{
            {1.0F, -1.0F}, {2.0F, -2.0F}};

        // By row
        constexpr matrix<vector<float, 2>, 2> m_by_row{
            1.0F, 2.0F, -1.0F, -2.0F};

        expect(constant<eq(m_by_column, m_by_row)>);
    };
}

// NOLINTEND(readability-magic-numbers)
