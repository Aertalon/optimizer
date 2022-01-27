#pragma once

#include "src/dualnumbers/dualnumbers.hpp"

#include <cmath>

namespace dualnumbers {

template <std::size_t N>
constexpr std::size_t factorial()
{
    return N * factorial<N - 1>();
}

template <>
constexpr std::size_t factorial<0>()
{
    return 1;
}

template <std::size_t N>
struct power {
    template <class T>
    constexpr T operator()(T const& x)
    {
        return x * power<N - 1>{}(x);
    }
};

template <>
struct power<0> {
    template <class T>
    constexpr T operator()(T const& x)
    {
        return T{1};
    }
};

// Dumb exponential for exposition only
template <std::size_t... Is>
constexpr float exp_impl(float const& x, std::index_sequence<Is...>)
{
    using unused = int[];
    float ret{};

    static_cast<void>(unused{
        0,
        (ret += power<Is>{}(x) / static_cast<float>(factorial<Is>()), 0)...});

    return ret;
}

constexpr float exp(float const& x)
{
    return exp_impl(x, std::make_index_sequence<10>{});
}

constexpr DualNumber exp(DualNumber const& x)
{
    return {exp(x.real()), x.imag() * exp(x.real())};
}

}  // namespace dualnumbers
