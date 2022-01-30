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
        di[i].imag1 = 1.0F;

        r[i] = cost(di).imag1;
    }

    return r;
}

template <Point P, Cost<P> F>
constexpr auto line_search(P orig, distance_t<P> direction, F cost) -> P
{
    // TODO(enrlov): Implement actual linesearch
    // FIXME don't assume scalar type is constructible from float
    constexpr scalar_t<P> lambda{0.001F};  // NOLINT(readability-magic-numbers)

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
    // FIXME don't assume scalar type is constructible from float
    constexpr scalar_t<V> tol{0.0001F};  // NOLINT(readability-magic-numbers)

    return opt::norm(g) < tol;
}

template <Point P, Cost<P> F>
constexpr auto optimize(P x, F c) -> P
{
    auto g{gradient(x, c)};

    constexpr std::size_t max_steps{10};
    for (std::size_t steps{0U}; not stopping_criterion(g) and steps < max_steps;
         ++steps) {
        x = line_search(std::move(x), -g, c);
        g = gradient(x, c);
    }

    return x;
}

}  // namespace opt
