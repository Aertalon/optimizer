#pragma once

#include "concepts.hpp"
#include "stdx/traits.hpp"

#include <iostream>

namespace opt {

template <Arithmetic T>
struct dual {
    // Names are temporary
    T real{};
    T e1{};
    T e2{};
    T e3{};

    [[nodiscard]] friend constexpr auto operator+(const dual& x, const dual& y)
        -> dual
    {
        return {x.real + y.real, x.e1 + y.e1, x.e2 + y.e2, x.e3 + y.e3};
    }

    [[nodiscard]] friend constexpr auto operator-(const dual& x, const dual& y)
        -> dual
    {
        return {x.real - y.real, x.e1 - y.e1, x.e2 - y.e2, x.e3 - y.e3};
    }

    [[nodiscard]] friend constexpr auto operator*(const dual& x, const dual& y)
        -> dual
    {
        return {x.real * y.real,
                x.real * y.e1 + x.e1 * y.real,
                x.real * y.e2 + x.e2 * y.real,
                x.real * y.e3 + x.e3 * y.real + x.e1 * y.e2 + x.e2 * y.e1};
    }

    [[nodiscard]] friend constexpr auto operator/(const dual& x, const dual& y)
        -> dual
    {
        auto den{y.real * y.real};
        return {x.real / y.real,
                (x.e1 * y.real - x.real * y.e1) / den,
                (x.e2 * y.real - x.real * y.e2) / den,
                ((2 * y.e1 * y.e2 - y.e3) * x.real / y.real + x.e3 * y.real -
                 x.e1 * y.e2 - x.e2 * y.e1) /
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
        os << "(" << x.real << ", " << x.e2 << ", " << x.e2 << ", " << x.e3
           << ")";
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

    [[nodiscard]] friend constexpr auto operator-(const dual& x) -> dual
    {
        return {-x.real, -x.e1, -x.e2, -x.e3};
    }

    [[nodiscard]] friend constexpr auto
    close_to(const dual& x, const dual& y, const T& tol) -> bool
    {
        constexpr auto abs = [](auto x) {
            if (x < decltype(x){}) {
                return -x;
            }

            return x;
        };

        return ((abs(x.real - y.real) < tol) && (abs(x.e1 - y.e1) < tol) &&
                (abs(x.e2 - y.e2) < tol) && (abs(x.e3 - y.e3) < tol));
    }
};

#ifdef __clang__
template <class T>
dual(T) -> dual<T>;
template <class T>
dual(T, T, T, T) -> dual<T>;
#endif

template <class T>
using is_dual = stdx::is_specialization_of<T, dual>;

template <class T>
inline constexpr bool is_dual_v = is_dual<T>::value;

template <class T>
concept Dual = is_dual_v<T>;

template <class T>
concept Real = Arithmetic<T> && not Dual<T>;

}  // namespace opt
