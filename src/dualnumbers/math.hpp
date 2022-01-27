#pragma once

#include "src/dualnumbers/dualnumbers.hpp"

#include <cmath>

namespace dualnumbers {

template <std::size_t N>
constexpr auto factorial() -> std::size_t
{
    return N * factorial<N - 1>();
}

template <>
constexpr auto factorial<0>() -> std::size_t
{
    return 1;
}

template <std::size_t N>
struct power {
    template <class T>
    [[nodiscard]] constexpr auto operator()(T const& x) const -> T
    {
        return x * power<N - 1>{}(x);
    }
};

template <>
struct power<0> {
    template <class T>
    [[nodiscard]] constexpr auto operator()(T const&) const -> T
    {
        return T{1};
    }
};

// Dumb exponential for exposition only
template <std::size_t... Is>
constexpr auto exp_impl(float const& x, std::index_sequence<Is...>) -> float
{
    using unused = int[];
    float ret{};

    static_cast<void>(unused{
        0,
        (ret += power<Is>{}(x) / static_cast<float>(factorial<Is>()), 0)...});

    return ret;
}

constexpr auto exp(float const& x) -> float
{
    // NOLINTNEXTLINE(readability-magic-numbers)
    return exp_impl(x, std::make_index_sequence<10>{});
}

constexpr auto exp(DualNumber const& x) -> DualNumber
{
    return {exp(x.real()), x.imag() * exp(x.real())};
}

}  // namespace dualnumbers
