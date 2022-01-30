#pragma once

#include "concepts.hpp"
#include "stdx/traits.hpp"

#include <iostream>

namespace opt {

template <Arithmetic T>
struct dual {
    // Names are temporary
    T real{};
    T imag1{};
    T imag2{};
    T imag3{};

    [[nodiscard]] friend constexpr auto operator+(const dual& x, const dual& y)
        -> dual
    {
        return {x.real + y.real,
                x.imag1 + y.imag1,
                x.imag2 + y.imag2,
                x.imag3 + y.imag3};
    }

    [[nodiscard]] friend constexpr auto operator-(const dual& x, const dual& y)
        -> dual
    {
        return {x.real - y.real,
                x.imag1 - y.imag1,
                x.imag2 - y.imag2,
                x.imag3 - y.imag3};
    }

    [[nodiscard]] friend constexpr auto operator*(const dual& x, const dual& y)
        -> dual
    {
        return {x.real * y.real,
                x.real * y.imag1 + x.imag1 * y.real,
                x.real * y.imag2 + x.imag2 * y.real,
                x.real * y.imag3 + x.imag3 * y.real + x.imag1 * y.imag2 +
                    x.imag2 * y.imag1};
    }

    [[nodiscard]] friend constexpr auto operator/(const dual& x, const dual& y)
        -> dual
    {
        auto den{y.real * y.real};
        return {x.real / y.real,
                (x.imag1 * y.real - x.real * y.imag1) / den,
                (x.imag2 * y.real - x.real * y.imag2) / den,
                ((2 * y.imag1 * y.imag2 - y.imag3) * x.real / y.real +
                 x.imag3 * y.real - x.imag1 * y.imag2 - x.imag2 * y.imag1) /
                    den};
    }

    constexpr auto operator+=(const dual& x) -> dual&
    {
        return *this = *this + x;
    }
    constexpr auto operator-=(const dual& x) -> dual&
    {
        return *this = *this - x;
    }
    constexpr auto operator*=(const dual& x) -> dual&
    {
        return *this = *this * x;
    }
    constexpr auto operator/=(const dual& x) -> dual&
    {
        return *this = *this / x;
    }

    friend auto operator<<(std::ostream& os, const dual& x) -> std::ostream&
    {
        os << "(" << x.real << ", " << x.imag2 << ", " << x.imag2 << ", "
           << x.imag3 << ")";
        return os;
    }

    // TODO: this is probably wrong, fix this
    [[nodiscard]] friend constexpr auto operator<(const dual& x, const dual& y)
        -> bool
    {
        return x.real < y.real;
    }

    [[nodiscard]] friend constexpr auto operator==(const dual& x, const dual& y)
        -> bool = default;

    [[nodiscard]] friend constexpr auto
    close_to(const dual& x, const dual& y, const T& tol) -> bool
    {
        constexpr auto abs = [](auto x) {
            if (x < decltype(x){}) {
                return -x;
            }

            return x;
        };

        return (
            (abs(x.real - y.real) < tol) && (abs(x.imag1 - y.imag1) < tol) &&
            (abs(x.imag2 - y.imag2) < tol) && (abs(x.imag3 - y.imag3) < tol));
    }

    [[nodiscard]] friend constexpr auto operator-(const dual& x) -> dual
    {
        return {-x.real, -x.imag1, -x.imag2, -x.imag3};
    }
};

#ifdef __clang__
template <class T>
dual(T) -> dual<T>;
template <class T>
dual(T, T) -> dual<T>;
#endif

// TODO(enrlov): add constraint -> dual should be built upon Arithmetic
template <class T>
using is_dual = stdx::is_specialization_of<T, dual>;

template <class T>
inline constexpr bool is_dual_v = is_dual<T>::value;

template <class T>
concept Dual = is_dual_v<T>;

template <class T>
concept Real = Arithmetic<T> && not Dual<T>;

}  // namespace opt
