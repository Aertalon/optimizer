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
        // By column - with vectors
        constexpr matrix<float, 3, 2> m_by_column{
            vector<float, 3>{1.0F, -1.0F, 0.0F},
            vector<float, 3>{2.0F, -2.0F, 0.0F}};

        // By column - init lists
        constexpr matrix<float, 3, 2> m_by_column_2{
            {1.0F, -1.0F, 0.0F}, {2.0F, -2.0F, 0.0F}};
        expect(constant<eq(m_by_column, m_by_column_2)>);

        // By row
        constexpr matrix<float, 3, 2> m_by_row{
            {1.0F, 2.0F}, {-1.0F, -2.0F}, {0.0F, 0.0F}};

        expect(constant<eq(m_by_column, m_by_row)>);
    };

    test("square matrix ctor") = [] {
        // By column - with vectors
        constexpr matrix<float, 2, 2> m_by_column{
            opt::from_columns{},
            vector<float, 2>{1.0F, -1.0F},
            vector<float, 2>{2.0F, -2.0F}};

        // By column - init lists
        constexpr matrix<float, 2, 2> m_by_column_2{
            opt::from_columns{}, {1.0F, -1.0F}, {2.0F, -2.0F}};
        expect(constant<eq(m_by_column, m_by_column_2)>);

        // By row
        constexpr matrix<float, 2, 2> m_by_row{
            opt::from_rows{}, {1.0F, 2.0F}, {-1.0F, -2.0F}};

        expect(constant<eq(m_by_column, m_by_row)>);
    };

    test("matrix product with vector on the right") = [] {
        constexpr matrix<float, 3, 2> m{
            {1.0F, -1.0F, 0.0F}, {2.0F, -2.0F, 0.0F}};

        constexpr matrix<float, 2, 1> v{{1.0F, -1.0F}};
        constexpr matrix<float, 3, 1> expected{{-1.0F, 1.0F, 0.0F}};

        expect(constant<eq(m * v, expected)>);
    };

    test("matrix transpose") = [] {
        constexpr matrix<float, 3, 2> m{
            {1.0F, -1.0F, 0.0F}, {2.0F, -2.0F, 0.0F}};

        constexpr matrix<float, 2, 3> mt{
            {1.0F, 2.0F}, {-1.0F, -2.0F}, {0.0F, 0.0F}};

        expect(constant<eq(m.transpose(), mt)>);
    };

    test("matrix sum") = [] {
        constexpr matrix<float, 3, 2> m0{
            {1.0F, -1.0F}, {2.0F, -2.0F}, {3.0F, -3.0F}};

        constexpr matrix<float, 3, 2> m1{
            {41.0F, 43.0F}, {40.0F, 44.0F}, {39.0F, 45.0F}};

        constexpr matrix<float, 3, 2> expected{
            {42.0F, 42.0F, 42.0F}, {42.0F, 42.0F, 42.0F}};

        expect(constant<eq(m0 + m1, expected)>);
    };

    test("matrix identity") = [] {
        constexpr matrix<float, 3, 3> expected{
            opt::from_columns{},
            {1.0F, 0.0F, 0.0F},
            {0.0F, 1.0F, 0.0F},
            {0.0F, 0.0F, 1.0F}};
        expect(constant<eq(expected, opt::identity<float, 3>)>);
    };

    test("matrix trace") = [] {
        constexpr matrix<float, 3, 3> m{
            opt::from_columns{},
            {1.0F, -42.0F, -42.0F},
            {-42.0F, 2.0F, -42.0F},
            {-42.0F, -42.0F, 3.0F}};
        expect(constant<eq(trace(m), 6.0F)>);
    };
}

// NOLINTEND(readability-magic-numbers)
