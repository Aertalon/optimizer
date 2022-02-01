#pragma once

#include "src/concepts.hpp"
#include "src/dualnumbers.hpp"
#include "src/spaces.hpp"

#include <algorithm>
#include <array>
#include <iostream>

namespace opt {

template <Point P, Cost<P> F>
constexpr auto gradient(const P& p, F cost) -> distance_t<P>
{
    constexpr auto N = std::tuple_size_v<P>;

    const auto d = [&p]<std::size_t... Is>(std::index_sequence<Is...>)
    {
        return opt::point{dual{p[Is]}...};
    }
    (std::make_index_sequence<N>{});

    const auto di = [&d](auto n) {
        auto tmp = d;
        tmp[n].imag = 1;
        return tmp;
    };

    return [&di, cost ]<std::size_t... Is>(std::index_sequence<Is...>)
    {
        return distance_t<P>{
            cost(di(std::integral_constant<std::size_t, Is>{})).imag...};
    }
    (std::make_index_sequence<N>{});
}

template <Point P, Cost<P> F>
constexpr auto line_search(P orig, distance_t<P> direction, F cost) -> P
{
    // TODO(enrlov): Implement actual linesearch

    using T = scalar_t<P>;
    constexpr auto lambda =
        T{1} / T{1'000};  // NOLINT(readability-magic-numbers)

    while (true) {
        auto end = orig + lambda * direction;

        if (cost(end) >= cost(orig)) {
            return orig;
        }

        std::exchange(orig, std::move(end));
    }
}

template <Vector V>
constexpr auto stopping_criterion(const V& g) -> bool
{
    using T = scalar_t<V>;
    constexpr auto tol = T{1} / T{1'000};  // NOLINT(readability-magic-numbers)

    return opt::norm(g) < tol;
}

template <Point P, Cost<P> F>
constexpr auto optimize(P x, F c, std::size_t max_steps = 10U) -> P
{
    auto g{gradient(x, c)};

    while (not stopping_criterion(g) or max_steps-- != 0U) {
        x = line_search(std::move(x), -g, c);
        g = gradient(x, c);
    }

    return x;
}

}  // namespace opt
