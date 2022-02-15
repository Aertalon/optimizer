#include "src/matrix.hpp"
#include "src/spaces.hpp"

#include "boost/ut.hpp"

// NOLINTBEGIN(readability-magic-numbers)

template <class T, std::size_t Rows, std::size_t Cols>
using mat = opt::matrix<opt::vector<T, Rows>, Cols>;

auto main() -> int
{
    using namespace boost::ut;

    using opt::matrix;
    using opt::vector;

    test("rectangular matrix ctor") = [] {
        // By column
        constexpr mat<float, 3, 2> m_by_column_vectors{
            vector<float, 3>{1.0F, -1.0F, 0.0F},
            vector<float, 3>{2.0F, -2.0F, 0.0F}};

        // By column - init lists
        constexpr mat<float, 3, 2> m_by_column_init_lists{
            {1.0F, -1.0F, 0.0F}, {2.0F, -2.0F, 0.0F}};
        expect(constant<eq(m_by_column_vectors, m_by_column_init_lists)>);

        // By row
        // clang-format off
        constexpr mat<float, 3, 2> m_by_rows{
            1.0F, 2.0F, -1.0F,
            -2.0F, 0.0F, 0.0F};
        // clang-format on

        expect(constant<eq(m_by_column_vectors, m_by_rows)>);
    };

    test("matrix indices") = [] {
        // clang-format off
        constexpr mat<float, 2, 2> m{
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

    test("matrix sum") = [] {
        // clang-format off
        constexpr mat<float, 3, 2> m0{
            1.0F, 2.0F,
            3.0F, -1.0F,
            -2.0F, -3.0F};

        constexpr mat<float, 3, 2> m1{
            41.0F, 40.0F,
            39.0F, 43.0F,
            44.0F, 45.0F};

        constexpr mat<float, 3, 2> expected{
            42.0F, 42.0F,
            42.0F, 42.0F,
            42.0F, 42.0F};
        // clang-format on
        expect(constant<eq(m0 + m1, expected)>);
    };

    test("matrix product with vector on the right") = [] {
        // clang-format off
        constexpr mat<float, 3, 2> m0{
            1.0F, 2.0F,
            3.0F, -1.0F,
            -2.0F, -3.0F};
        // clang-format on

        constexpr vector<float, 2> v{1.0F, -1.0F};
        constexpr vector<float, 3> expected_from_vector{-1.0F, 4.0F, 1.0F};
        expect(constant<eq(m0 * v, expected_from_vector)>);

        constexpr mat<float, 2, 1> m1{1.0F, -1.0F};
        constexpr mat<float, 3, 1> expected_from_matrix{-1.0F, 4.0F, 1.0F};
        expect(constant<eq(m0 * m1, expected_from_matrix)>);
    };

    test("matrix product") = [] {
        // clang-format off
        constexpr mat<float, 3, 2> m0{
            1.0F, 2.0F,
            3.0F, -1.0F,
            -2.0F, -3.0F};

        constexpr mat<float, 2, 2> m1{
            -1.0F, 1.0F,
            3.0F, 2.0F};

        constexpr mat<float, 3, 2> expected{
            5.0F, 5.0F,
            -6.0F, 1.0F,
            -7.0F, -8.0F};
        // clang-format on
        expect(constant<eq(m0 * m1, expected)>);
    };
}

// NOLINTEND(readability-magic-numbers)
