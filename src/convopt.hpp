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
    using T = scalar_t<P>;

    const auto d = [&p]() {
        auto d = opt::point<dual<T>, N>{};

        for (std::size_t i{0U}; i < N; ++i) {
            d[i].real = p[i];
        }

        return d;
    }();

    auto r = distance_t<P>{};

    for (std::size_t i{0U}; i < N; ++i) {
        auto di = d;
        di[i].imag = 1;
        r[i] = cost(di).imag;
    }

    return r;
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
