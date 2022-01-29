#pragma once

#include <iostream>

namespace opt {

struct DualNumber {
    float real{};
    float imag{};

    [[nodiscard]] friend constexpr auto
    operator+(DualNumber const& x, DualNumber const& y) -> DualNumber
    {
        return {x.real + y.real, x.imag + y.imag};
    }

    [[nodiscard]] friend constexpr auto
    operator-(DualNumber const& x, DualNumber const& y) -> DualNumber
    {
        return {x.real - y.real, x.imag - y.imag};
    }

    [[nodiscard]] friend constexpr auto
    operator*(DualNumber const& x, DualNumber const& y) -> DualNumber
    {
        return {x.real * y.real, x.real * y.imag + x.imag * y.real};
    }

    [[nodiscard]] friend constexpr auto
    operator/(DualNumber const& x, DualNumber const& y) -> DualNumber
    {
        return {x.real / y.real,
                (x.imag * y.real - x.real * y.imag) / (y.real * y.real)};
    }

    friend auto operator<<(std::ostream& os, DualNumber const& x)
        -> std::ostream&
    {
        os << "(" << x.real << ", " << x.imag << ")";
        return os;
    }

    // TODO: this is probably wrong, fix this
    [[nodiscard]] friend constexpr auto
    operator<(DualNumber const& x, DualNumber const& y) -> bool
    {
        return x.real < y.real;
    }

    [[nodiscard]] friend constexpr auto
    operator==(DualNumber const& x, DualNumber const& y) -> bool = default;

    [[nodiscard]] friend constexpr auto operator-(DualNumber const& x)
        -> DualNumber
    {
        return {-x.real, -x.imag};
    }
};

}  // namespace opt
