#pragma once

#include "concepts.hpp"
#include "dualnumbers.hpp"

#include <cmath>
#include <cstddef>
#include <type_traits>

namespace opt {

template <Arithmetic T>  // clang-format off
  requires(not Dual<T>)
constexpr auto exp(T x)  // clang-format on
{
    auto acc = decltype(x){1};
    auto power = acc;
    auto factorial = acc;

    constexpr std::size_t taylor_terms{10U};
    for (std::size_t i{1U}; i < taylor_terms; ++i) {
        power *= x;
        factorial *= static_cast<decltype(factorial)>(i);

        acc += power / factorial;
    }
    return acc;
}

constexpr auto exp(Dual auto const& x)
{
    return std::remove_cvref_t<decltype(x)>{exp(x.real), x.imag * exp(x.real)};
}

}  // namespace opt
