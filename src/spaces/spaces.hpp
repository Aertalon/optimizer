#pragma once

#include <algorithm>
#include <array>
#include <concepts>
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
        // TODO move to common location or replace with constexpr math lib
        constexpr auto abs = [](auto x) {
            if (x < decltype(x){}) {
                return -x;
            }

            return x;
        };

        return std::transform_reduce(
            lhs.cbegin(),
            lhs.end(),
            rhs.cbegin(),
            true,
            std::logical_and{},
            [abs, tol](auto x, auto y) { return abs(x - y) < tol; });
    }

    friend auto operator<<(std::ostream& os, Entity const& p) -> std::ostream&
    {
        os << "(";
        os << p.coords()[0];

        // TODO use `std::views::drop` once clang implements it
        for (auto x : std::ranges::drop_view{p, 1}) {
            os << ", " << x;
        }

        os << ")";
        return os;
    }

    [[nodiscard]] friend constexpr auto
    operator==(Entity const& lhs, Entity const& rhs) -> bool = default;

    friend Derived<T, N>;

    constexpr Entity() = default;

    constexpr Entity(coords_type coords) : coords_type{std::move(coords)} {}

    // NOLINTBEGIN(modernize-use-equals-delete)
    template <class... Args>
    requires(std::same_as<coord_type, std::remove_cvref_t<Args>>&&...)  //
        explicit(sizeof...(Args) == 1) constexpr Entity(Args&&... xs)
        : coords_type({std::forward<Args>(xs)...})
    {}
    // NOLINTEND(modernize-use-equals-delete)
};

/// A simple vector class
template <class T, std::size_t N>
class Vector : public Entity<Vector<T, N>> {
  public:
    static constexpr std::size_t size = N;
    using coord_type = typename Entity<Vector<T, N>>::coord_type;

    Vector() = default;

    template <class... Args>
    requires(std::same_as<coord_type, std::remove_cvref_t<Args>>&&...)  //
        explicit(sizeof...(Args) == 1) constexpr Vector(Args&&... args)
        : Entity<Vector<T, N>>(std::forward<Args>(args)...)
    {}

  private:
    [[nodiscard]] friend constexpr auto operator-(Vector const& v) -> Vector
    {
        auto r = Vector{};
        std::transform(v.cbegin(), v.cend(), r.begin(), std::negate{});
        return r;
    }

    [[nodiscard]] friend constexpr auto
    operator+(Vector const& v1, Vector const& v2) -> Vector
    {
        auto r = Vector{};
        std::transform(
            v1.cbegin(), v1.cend(), v2.cbegin(), r.begin(), std::plus{});
        return r;
    }

    [[nodiscard]] friend constexpr auto
    operator*(Vector const& v, Vector::coord_type const& s) -> Vector
    {
        auto r = Vector{};
        std::transform(v.cbegin(), v.cend(), r.begin(), [s](auto x) {
            return s * x;
        });
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
        std::transform(
            p.cbegin(), p.cend(), v.cbegin(), r.begin(), std::plus{});
        return r;
    }
};

}  // namespace spaces
