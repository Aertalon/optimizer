#pragma once

#include "src/concepts.hpp"
#include "src/dualnumbers.hpp"
#include "src/spaces.hpp"

#include <algorithm>
#include <array>
#include <execution>
#include <iostream>
#include <utility>

namespace opt {
namespace detail {

template <std::size_t N, class R = std::array<std::size_t, N>>
[[nodiscard]] consteval auto index_array() -> R
{
    auto is = R{};

    for (auto n : std::views::iota(std::size_t{}, N)) {
        is[n] = n;
    }

    return is;
}

template <Point P,
          class R = opt::point<dual<scalar_t<P>>, std::tuple_size_v<P>>>
[[nodiscard]] constexpr auto as_point_dual(const P& p) -> R
{
    if constexpr (std::is_same_v<P, R>) {
        return p;
    }

    constexpr auto N = std::tuple_size_v<P>;
    auto to = R{};

    for (auto i : std::views::iota(std::size_t{}, N)) {
        to[i].real = p[i];
    }

    return to;
}

}  // namespace detail

template <Point P, Cost<P> F>
constexpr auto gradient(const P& p, F cost) -> distance_t<P>
{
    constexpr auto N = std::tuple_size_v<P>;

    auto r = distance_t<P>{};
    auto set_ith = [&r, &cost, d = detail::as_point_dual(p)](auto i) {
        auto di = d;
        di[i].e1 = 1;
        r[i] = cost(di).e1;
    };

    constexpr auto idx = detail::index_array<N>();
#ifdef __cpp_lib_execution
    if (not std::is_constant_evaluated()) {
        std::for_each(
            std::execution::par_unseq,
            idx.cbegin(),
            idx.cend(),
            std::move(set_ith));
        return r;
    }
#endif
    std::for_each(idx.cbegin(), idx.cend(), std::move(set_ith));
    return r;
}

template <Point P, Cost<P> F>
constexpr auto hessian(const P& p, F cost)
{
    constexpr auto N = std::tuple_size_v<P>;

    auto h = std::array<distance_t<P>, N>{};
    auto set_ijth = [&h, &cost, d = detail::as_point_dual(p)](auto ij) {
        const auto i = ij / N;
        const auto j = ij % N;

        auto dij = d;
        dij[i].e1 = 1;
        dij[j].e2 = 1;
        // FIXME(enrlov): properly index matrices when available
        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
        h[i][j] = cost(dij).e3;
    };

    constexpr auto ij = detail::index_array<N * N>();
#ifdef __cpp_lib_execution
    if (not std::is_constant_evaluated()) {
        std::for_each(
            std::execution::par_unseq,
            ij.cbegin(),
            ij.cend(),
            std::move(set_ijth));
        return h;
    }
#endif
    std::for_each(ij.cbegin(), ij.cend(), std::move(set_ijth));
    return h;
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
