#include <iostream>

#ifndef DUALNUMBERS_H_
#define DUALNUMBERS_H_

namespace dualnumbers
{

class DualNumber
{
    private:
        float r_{};
        float e_{};

    friend constexpr DualNumber operator+(DualNumber const& x, DualNumber const& y)
    {
        return DualNumber{x.real() + y.real(), x.imag() + y.imag()};
    }

    friend constexpr DualNumber operator-(DualNumber const& x, DualNumber const& y)
    {
        return DualNumber{x.real() - y.real(), x.imag() - y.imag()};
    }

    friend constexpr DualNumber operator*(DualNumber const& x, DualNumber const& y)
    {
        return DualNumber{x.real()*y.real(), x.real()*y.imag() + x.imag()*y.real()};
    }

    friend constexpr DualNumber operator/(DualNumber const& x, DualNumber const& y)
    {
        return DualNumber{x.real()/y.real(), (x.imag()*y.real() - x.real()*y.imag())/(y.real()*y.real())};
    }


    friend std::ostream& operator<<(std::ostream& os, DualNumber const& x)
    {
        os << "(" << x.real() << ", " << x.imag() << ")";
        return os;
    }

    // TODO: this is probably wrong, fix this
    friend constexpr bool operator<(DualNumber const& x, DualNumber const& y)
    {
        return x.real() < y.real();
    }

    friend constexpr bool operator==(DualNumber const& x, DualNumber const& y)
    {
        return (x.real() == y.real()) && (x.imag() == y.imag());
    }

    friend constexpr bool operator!=(DualNumber const& x, DualNumber const& y)
    {
        return !(x == y);
    }

    friend constexpr DualNumber operator-(DualNumber const& x)
    {
        return DualNumber{-x.real(), -x.real()};
    }

    public:
        constexpr DualNumber() = default;

        constexpr DualNumber(float r) :
         r_{r},
         e_{}
        {}

        constexpr DualNumber(float r, float e) :
         r_{r},
         e_{e}
        {}

        constexpr float real() { return r_; }
        constexpr float real() const { return r_; }

        constexpr float imag() { return e_; }
        constexpr float imag() const { return e_; }
};

}  // namespace dualnumbers

#endif // DUALNUMBERS_H_