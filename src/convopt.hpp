#pragma once

#include "src/concepts.hpp"
#include "src/dualnumbers.hpp"
#include "src/spaces.hpp"

#include <algorithm>
#include <array>
#include <iostream>

namespace opt {

// TODO(enrlov): F must be invocable with T
template <std::size_t N, class T, class F>
constexpr auto gradient(point<T, N> p, F cost) -> distance_t<point<T, N>>
{
    // TODO(olee): Should point also be a range?
    // TODO(olee): Should point also be sized?
    // TODO(olee): Should point be rebindable?

    // using T = scalar_t<distance_t<decltype(p)>>;

    const auto d = [&p]() {
        auto d = point<dual<T>, N>{};
        std::transform(p.cbegin(), p.cend(), d.begin(), [](auto x) {
            return dual<T>{.real = x};
        });
        return d;
    }();

    auto r = distance_t<point<T, N>>{};

    for (auto i = 0U; i < N; ++i) {
        auto di = d;
        di[i].imag = 1.0F;

        r[i] = cost(di).imag;
    }

    return r;
}

// TODO(enrlov): F must be invocable with T
template <Point P, class F>
constexpr auto line_search(P orig, distance_t<P> direction, F cost) -> P
{
    // Implement actual linesearch
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
    constexpr scalar_t<V> tol{0.001F};  // NOLINT(readability-magic-numbers)
    return norm(g) < tol;
}

// TODO(enrlov): F must be invocable with T
template <Point P, class F>
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
