#pragma once

#include <iostream>

namespace dualnumbers {

class DualNumber {
  private:
    float r_{};
    float e_{};

    friend constexpr auto operator+(DualNumber const& x, DualNumber const& y)
        -> DualNumber
    {
        return {x.real() + y.real(), x.imag() + y.imag()};
    }

    friend constexpr auto operator-(DualNumber const& x, DualNumber const& y)
        -> DualNumber
    {
        return {x.real() - y.real(), x.imag() - y.imag()};
    }

    friend constexpr auto operator*(DualNumber const& x, DualNumber const& y)
        -> DualNumber
    {
        return {x.real() * y.real(), x.real() * y.imag() + x.imag() * y.real()};
    }

    friend constexpr auto operator/(DualNumber const& x, DualNumber const& y)
        -> DualNumber
    {
        return {x.real() / y.real(),
                (x.imag() * y.real() - x.real() * y.imag()) /
                    (y.real() * y.real())};
    }

    friend auto operator<<(std::ostream& os, DualNumber const& x)
        -> std::ostream&
    {
        os << "(" << x.real() << ", " << x.imag() << ")";
        return os;
    }

    // TODO: this is probably wrong, fix this
    friend constexpr auto operator<(DualNumber const& x, DualNumber const& y)
        -> bool
    {
        return x.real() < y.real();
    }

    friend constexpr auto operator==(DualNumber const& x, DualNumber const& y)
        -> bool
    {
        return (x.real() == y.real()) && (x.imag() == y.imag());
    }

    friend constexpr auto operator!=(DualNumber const& x, DualNumber const& y)
        -> bool
    {
        return !(x == y);
    }

    friend constexpr auto operator-(DualNumber const& x) -> DualNumber
    {
        // TODO(enrlov): is this right?
        return {-x.real(), -x.real()};
    }

  public:
    DualNumber() = default;

    explicit constexpr DualNumber(float r) : r_{r} {}

    // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
    constexpr DualNumber(float r, float e) : r_{r}, e_{e} {}

    [[nodiscard]] constexpr auto real() & -> float& { return r_; }
    [[nodiscard]] constexpr auto real() const& -> const float& { return r_; }

    [[nodiscard]] constexpr auto imag() & -> float& { return e_; }
    [[nodiscard]] constexpr auto imag() const& -> const float& { return e_; }
};

}  // namespace dualnumbers
