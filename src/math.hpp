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

template <Arithmetic T>  // clang-format off
  requires(not Dual<T>)
constexpr auto cos(T x)  // clang-format on
{
    auto acc = decltype(x){1};
    auto power = acc;
    auto factorial = acc;

    constexpr std::size_t taylor_terms{10U};
    for (std::size_t i{1U}; i < taylor_terms; ++i) {
        power *= x * x;
        factorial *= static_cast<decltype(factorial)>((2 * i - 1) * (2 * i));

        acc +=
            (i % 2 == 0 ? decltype(x){1} : -decltype(x){1}) * power / factorial;
    }
    return acc;
}

template <Arithmetic T>  // clang-format off
  requires(not Dual<T>)
constexpr auto sin(T x)  // clang-format on
{
    auto acc = x;
    auto power = x;
    auto factorial = decltype(x){1};

    constexpr std::size_t taylor_terms{10U};
    for (std::size_t i{1U}; i < taylor_terms; ++i) {
        power *= x * x;
        factorial *= static_cast<decltype(factorial)>((2 * i) * (2 * i + 1));

        acc +=
            (i % 2 == 0 ? decltype(x){1} : -decltype(x){1}) * power / factorial;
    }
    return acc;
}

constexpr auto exp(Dual auto const& x)
{
    return std::remove_cvref_t<decltype(x)>{exp(x.real), x.imag * exp(x.real)};
}

constexpr auto cos(Dual auto const& x)
{
    return std::remove_cvref_t<decltype(x)>{cos(x.real), -x.imag * sin(x.real)};
}

constexpr auto sin(Dual auto const& x)
{
    return std::remove_cvref_t<decltype(x)>{sin(x.real), x.imag * cos(x.real)};
}

}  // namespace opt
