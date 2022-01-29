#pragma once

#include "concepts.hpp"
#include "stdx/traits.hpp"

#include <iostream>

namespace opt {

// TODO replace with Real number concept
template <Arithmetic T>
struct dual {
    T real{};
    T imag{};

    [[nodiscard]] friend constexpr auto operator+(const dual& x, const dual& y)
        -> dual
    {
        return {x.real + y.real, x.imag + y.imag};
    }

    [[nodiscard]] friend constexpr auto operator-(const dual& x, const dual& y)
        -> dual
    {
        return {x.real - y.real, x.imag - y.imag};
    }

    [[nodiscard]] friend constexpr auto operator*(const dual& x, const dual& y)
        -> dual
    {
        return {x.real * y.real, x.real * y.imag + x.imag * y.real};
    }

    [[nodiscard]] friend constexpr auto operator/(const dual& x, const dual& y)
        -> dual
    {
        return {x.real / y.real,
                (x.imag * y.real - x.real * y.imag) / (y.real * y.real)};
    }

    friend auto operator<<(std::ostream& os, const dual& x) -> std::ostream&
    {
        os << "(" << x.real << ", " << x.imag << ")";
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
        return {-x.real, -x.imag};
    }
};

#ifdef __clang__
template <class T>
dual(T) -> dual<T>;
template <class T>
dual(T, T) -> dual<T>;
#endif

template <class T>
using is_dual = stdx::is_specialization_of<T, dual>;

template <class T>
inline constexpr bool is_dual_v = is_dual<T>::value;

template <class T>
concept Dual = is_dual_v<T>;

}  // namespace opt
