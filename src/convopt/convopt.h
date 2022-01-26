#include <iostream>
#include <algorithm>
#include <array>
#include "src/dualnumbers/dualnumbers.h"
#include "src/spaces/spaces.h"


#ifndef CONVOPT_H_
#define CONVOPT_H_

namespace convopt
{

using dualnumbers::DualNumber;
using spaces::Point;
using spaces::Vector;



template<std::size_t I, std::size_t N, class T, std::size_t... Is>
constexpr Point<DualNumber, N> evaluated_point_at_index(Point<T, N> x, std::index_sequence<Is...>)
{
    Point<DualNumber, N> point{DualNumber{x.template get<Is>(), Is == I ? 1.0F : 0.0F}...};
    return point;
}

template<std::size_t N, class T, class F, std::size_t... Is>
constexpr Vector<T, N> gradient_impl(Point<T, N> x, F cost, std::index_sequence<Is...>)
{
    return {(cost(evaluated_point_at_index<Is>(x, std::make_index_sequence<N>{})).imag())...};
}

template<std::size_t N, class T, class F>
constexpr Vector<T, N> gradient(Point<T, N> x, F cost)
{
    return gradient_impl(x, cost, std::make_index_sequence<N>{});
}

template<std::size_t N, class T, class F>
constexpr Point<T, N> line_search(Point<T, N> orig, Vector<T, N> direction, F cost)
{
    // Implement actual linesearch
    T lambda{0.001F};
    Point<T, N> end{orig};
    while (cost(end+lambda*direction) < cost(end))
    {
        end = end + lambda*direction;
    }
    return end;
}

template<std::size_t N, class T>
constexpr bool stopping_criterion(Vector<T, N> const& g)
{
    float tol{0.001F};
    return norm(g) < tol;
}

// TODO(enrlov): F must be invocable with T
template<std::size_t N, class T, class F>
constexpr auto optimize(Point<T, N> initial, F c)
{
    auto x{initial};
    auto g{gradient(x, c)};
    std::size_t steps{};
    constexpr std::size_t max_steps{10};

    while (!stopping_criterion(g) && (steps < max_steps))
    {
        x = line_search(x, -g, c);
        g = gradient(x, c);
        steps++;
    }

    return x;
}

}  // namespace convopt

#endif // CONVOPT_H_