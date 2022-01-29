#pragma once

#include "src/dualnumbers.hpp"

#include <cmath>
#include <concepts>
#include <type_traits>

namespace opt {

constexpr auto exp(std::floating_point auto x)
{
    auto acc = decltype(x){1};
    auto power = acc;
    auto factorial = acc;

    constexpr std::size_t taylor_terms{10U};
    for (std::size_t i{1U}; i < taylor_terms; ++i) {
        power *= x;
        factorial *= i;

        acc += power / factorial;
    }
    return acc;
}

constexpr auto exp(Dual auto const& x)
{
    return std::remove_cvref_t<decltype(x)>{exp(x.real), x.imag * exp(x.real)};
}

}  // namespace opt
