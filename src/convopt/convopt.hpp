#pragma once

#include "src/dualnumbers/dualnumbers.hpp"
#include "src/spaces/spaces.hpp"

#include <algorithm>
#include <array>
#include <iostream>

namespace convopt {

using dualnumbers::DualNumber;
using spaces::Point;
using spaces::Vector;

template <std::size_t N, class T, class F>
constexpr auto gradient(Point<T, N> p, F cost) -> Vector<T, N>
{
    const auto d = [&p]() {
        auto d = Point<DualNumber, N>{};
        std::transform(p.cbegin(), p.cend(), d.begin(), [](auto x) {
            return DualNumber{x};
        });
        return d;
    }();

    auto r = Vector<T, N>{};

    for (auto i = 0U; i < N; ++i) {
        auto di = d;
        di[i].imag() = 1.0F;

        r[i] = cost(di).imag();
    }

    return r;
}

template <std::size_t N, class T, class F>
constexpr auto line_search(Point<T, N> orig, Vector<T, N> direction, F cost)
    -> Point<T, N>
{
    // Implement actual linesearch
    // NOLINTNEXTLINE(readability-magic-numbers)
    T lambda{0.001F};
    Point<T, N> end = {orig};
    while (cost(end + lambda * direction) < cost(end)) {
        end = end + lambda * direction;
    }
    return end;
}

template <std::size_t N, class T>
constexpr auto stopping_criterion(Vector<T, N> const& g) -> bool
{
    // NOLINTNEXTLINE(readability-magic-numbers)
    float tol{0.001F};
    return norm(g) < tol;
}

// TODO(enrlov): F must be invocable with T
template <std::size_t N, class T, class F>
constexpr auto optimize(Point<T, N> initial, F c)
{
    auto x{initial};
    auto g{gradient(x, c)};
    std::size_t steps{};
    constexpr std::size_t max_steps{10};

    while (!stopping_criterion(g) && (steps < max_steps)) {
        x = line_search(x, -g, c);
        g = gradient(x, c);
        steps++;
    }

    return x;
}

}  // namespace convopt
