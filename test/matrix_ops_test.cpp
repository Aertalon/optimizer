#include "src/matrix_ops.hpp"
#include "src/norms.hpp"

#include "boost/ut.hpp"

// NOLINTBEGIN(readability-magic-numbers)

auto main() -> int
{
    using namespace boost::ut;

    using opt::matrix;

    test("matrix submatrix") = [] {
        constexpr matrix<float, 3, 2> m{
            {1.0F, 2.0F, 0.0F}, {-1.0F, -2.0F, 0.0F}};

        constexpr auto m0{opt::submatrix<0, 0>(m)};
        constexpr auto m1{opt::submatrix<0, 1>(m)};
        constexpr matrix<float, 3, 1> expected_m1{{-1.0F, -2.0F, 0.0F}};
        expect(constant<eq(m1, expected_m1)>);

        constexpr auto m2{opt::submatrix<1, 0>(m)};
        constexpr matrix<float, 2, 2> expected_m2{
            opt::from_columns{}, {2.0F, 0.0F}, {-2.0F, 0.0F}};
        expect(constant<eq(m2, expected_m2)>);

        constexpr auto m3{opt::submatrix<1, 1>(m)};
        constexpr matrix<float, 2, 1> expected_m3{{-2.0F, 0.0F}};
        expect(constant<eq(m3, expected_m3)>);
    };

    test("matrix diagonal") = [] {
        constexpr matrix<float, 1, 1> m0{opt::from_rows{}, {2.0F}};
        constexpr matrix<float, 3, 2> m1{
            {1.0F, -1.0F, 0.0F}, {2.0F, -2.0F, 0.0F}};
        constexpr matrix<float, 4, 3> actual_diag{diagonal(m0, m1)};

        constexpr matrix<float, 4, 3> expected_diag{
            {2.0F, 0.0F, 0.0F, 0.0F},
            {0.0F, 1.0F, -1.0F, 0.0F},
            {0.0F, 2.0F, -2.0F, 0.0F}};
        expect(constant<eq(actual_diag, expected_diag)>);
    };

    test("matrix qr factorization square") = [] {
        constexpr matrix<float, 3, 3> m{
            opt::from_rows{},
            {12.0F, -51.0F, 4.0F},
            {6.0F, 167.0F, -68.0F},
            {-4.0F, 24.0F, -41.0F}};

        constexpr auto qr{opt::compute_QR(m)};

        constexpr auto check_on_q{
            (qr.Q) * (qr.Q).transpose() - opt::identity<float, 3>};
        constexpr auto check_on_q_and_r{(qr.Q) * (qr.R) - m};

        // FIXME: these numerical bounds are bad. Improve!
        expect(constant<le(opt::norm<opt::Frobenius>(check_on_q), 1e-4F)>);
        expect(
            constant<le(opt::norm<opt::Frobenius>(check_on_q_and_r), 1e-3F)>);
    };

    test("matrix qr factorization rectangular") = [] {
        constexpr matrix<float, 4, 3> m{
            {12.0F, -51.0F, 4.0F},
            {6.0F, 167.0F, -68.0F},
            {-4.0F, 24.0F, -41.0F},
            {-1.0F, 2.0F, 1.0F},
        };

        constexpr auto qr{opt::compute_QR(m)};

        constexpr auto check_on_q{
            (qr.Q) * (qr.Q).transpose() - opt::identity<float, 4>};
        constexpr auto check_on_q_and_r{(qr.Q) * (qr.R) - m};

        // FIXME: these numerical bounds are bad. Improve!
        expect(constant<le(opt::norm<opt::Frobenius>(check_on_q), 1e-4F)>);
        expect(
            constant<le(opt::norm<opt::Frobenius>(check_on_q_and_r), 1e-3F)>);
    };

    test("matrix qr factorization rectangular") = [] {
        constexpr matrix<float, 2, 2> R{
            opt::from_rows{}, {1.0F, 2.0F}, {0.0F, -1.0F}};
        constexpr matrix<float, 2, 1> b{{1.0F, 1.0F}};

        constexpr matrix<float, 2, 1> expected{{3.0F, -1.0F}};
        constexpr auto actual{solve_upper_diagonal(R, b)};
        expect(constant<eq(actual, expected)>);
    };
}

// NOLINTEND(readability-magic-numbers)
