#pragma once

#include <algorithm>
#include <array>
#include <functional>
#include <iostream>
#include <numeric>
#include <ranges>
#include <utility>

namespace spaces {

template <class T>
class Entity {};

template <class T, std::size_t N, template <class, std::size_t> class Derived>
class Entity<Derived<T, N>> : std::array<T, N> {
  public:
    static constexpr std::size_t size{N};
    using coord_type = T;
    using coords_type = std::array<coord_type, size>;

    [[nodiscard]] constexpr auto coords() & -> coords_type&
    {
        return static_cast<coords_type&>(*this);
    }
    [[nodiscard]] constexpr auto coords() const& -> const coords_type&
    {
        return static_cast<const coords_type&>(*this);
    }

    template <std::size_t I>
    [[nodiscard]] constexpr auto get() const& -> const coord_type&
    {
        return std::get<I>(coords());
    }

    using coords_type::begin;
    using coords_type::cbegin;
    using coords_type::cend;
    using coords_type::end;
    using coords_type::operator[];

  private:
    [[nodiscard]] friend constexpr auto
    close_to(Entity const& lhs, Entity const& rhs, coord_type tol) -> bool
    {
        return std::transform_reduce(
            lhs.cbegin(),
            lhs.end(),
            rhs.cbegin(),
            true,
            std::logical_and{},
            [tol](auto x, auto y) { return std::abs(x - y) < tol; });
    }

    friend auto operator<<(std::ostream& os, Entity const& p) -> std::ostream&
    {
        os << "(";
        os << p.coords()[0];

        for (auto x : p | std::views::drop(1)) { os << ", " << x; }

        os << ")";
        return os;
    }

    [[nodiscard]] friend constexpr auto
    operator==(Entity const& lhs, Entity const& rhs) -> bool
    {
        return std::transform_reduce(
            lhs.cbegin(),
            lhs.cend(),
            rhs.cbegin(),
            true,
            std::logical_and{},
            std::ranges::equal_to{});
    }

    friend Derived<T, N>;

    constexpr Entity() = default;

    constexpr Entity(coords_type coords) : coords_type{std::move(coords)} {}

    template <class... Args>
    constexpr explicit Entity(coord_type x, Args&&... xs)
        : coords_type({x, xs...})
    {}
};

/// A simple vector class
template <class T, std::size_t N>
class Vector : public Entity<Vector<T, N>> {
  public:
    static constexpr std::size_t size = N;
    using coord_type = typename Entity<Vector<T, N>>::coord_type;

    Vector() = default;

    template <class... Args>
    constexpr Vector(Args&&... args)
        : Entity<Vector<T, N>>(std::forward<Args>(args)...)
    {}

  private:
    [[nodiscard]] friend constexpr auto operator-(Vector const& v) -> Vector
    {
        auto r = Vector{};
        std::ranges::transform(v, r.begin(), std::negate{});
        return r;
    }

    [[nodiscard]] friend constexpr auto
    operator+(Vector const& v1, Vector const& v2) -> Vector
    {
        auto r = Vector{};
        std::ranges::transform(v1, v2, r.begin(), std::plus{});
        return r;
    }

    [[nodiscard]] friend constexpr auto
    operator*(Vector const& v, Vector::coord_type const& s) -> Vector
    {
        auto r = Vector{};
        std::ranges::transform(v, r.begin(), [s](auto x) { return s * x; });
        return r;
    }

    [[nodiscard]] friend constexpr auto
    operator*(Vector::coord_type const& s, Vector const& v) -> Vector
    {
        return v * s;
    }

    [[nodiscard]] friend constexpr auto norm(Vector const& v)
    {
        return std::inner_product(
            v.cbegin(), v.cend(), v.cbegin(), Vector::coord_type{});
    }
};

/// A simple N-d point class
template <class T, std::size_t N>
class Point : public Entity<Point<T, N>> {
  public:
    static constexpr std::size_t size = N;

    Point() = default;

    template <class... Args>
    constexpr Point(Args&&... args) : Entity<Point>(std::forward<Args>(args)...)
    {}

  private:
    [[nodiscard]] friend constexpr auto
    operator+(Point const& p, Vector<T, N> const& v) -> Point
    {
        auto r = Point{};
        std::ranges::transform(p, v, r.begin(), std::plus{});
        return r;
    }
};

}  // namespace spaces
