#pragma once

#include "concepts.hpp"
#include "dualnumbers.hpp"

#include <cmath>
#include <cstddef>
#include <type_traits>

namespace opt {

template <Real T>
constexpr auto exp(T x) -> T
{
    auto acc = T{1};
    auto power = acc;
    auto factorial = acc;

    constexpr std::size_t taylor_terms{10U};
    for (std::size_t i{1U}; i < taylor_terms; ++i) {
        power *= x;
        factorial *= static_cast<T>(i);

        acc += power / factorial;
    }
    return acc;
}

template <Real T>
constexpr auto cos(T x) -> T
{
    auto acc = T{1};
    auto power = acc;
    auto factorial = acc;

    constexpr std::size_t taylor_terms{10U};
    for (std::size_t i{1U}; i < taylor_terms; ++i) {
        power *= x * x;
        factorial *= static_cast<T>((2 * i - 1) * (2 * i));

        acc += (i % 2 == 0 ? T{1} : -T{1}) * power / factorial;
    }
    return acc;
}

template <Real T>
constexpr auto sin(T x) -> T
{
    auto acc = x;
    auto power = x;
    auto factorial = T{1};

    constexpr std::size_t taylor_terms{10U};
    for (std::size_t i{1U}; i < taylor_terms; ++i) {
        power *= x * x;
        factorial *= static_cast<T>((2 * i) * (2 * i + 1));

        acc += (i % 2 == 0 ? T{1} : -T{1}) * power / factorial;
    }
    return acc;
}

template <Dual T>
constexpr auto exp(const T& x) -> T
{
    return T{exp(x.real), x.imag * exp(x.real)};
}

template <Dual T>
constexpr auto cos(const T& x) -> T
{
    return T{cos(x.real), -x.imag * sin(x.real)};
}

template <Dual T>
constexpr auto sin(const T& x) -> T
{
    return T{sin(x.real), x.imag * cos(x.real)};
}

}  // namespace opt
