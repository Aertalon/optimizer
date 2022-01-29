#pragma once

#include <algorithm>
#include <array>
#include <concepts>
#include <functional>
#include <iostream>
#include <numeric>
#include <ranges>
#include <utility>

namespace opt {

template <class T>
class Entity {};

template <class T, std::size_t N, template <class, std::size_t> class Derived>
struct Entity<Derived<T, N>> {
    using coords_type = std::array<T, N>;

    coords_type data{};

    [[nodiscard]] constexpr auto begin() & noexcept { return data.begin(); }
    [[nodiscard]] constexpr auto begin() const& noexcept
    {
        return data.begin();
    }
    [[nodiscard]] constexpr auto cbegin() const& noexcept
    {
        return data.cbegin();
    }

    [[nodiscard]] constexpr auto end() & noexcept { return data.end(); }
    [[nodiscard]] constexpr auto end() const& noexcept { return data.end(); }
    [[nodiscard]] constexpr auto cend() const& noexcept { return data.cend(); }

    [[nodiscard]] constexpr auto
    operator[](typename coords_type::size_type n) & -> auto&
    {
        return data[n];
    }
    [[nodiscard]] constexpr auto
    operator[](typename coords_type::size_type n) const& -> auto&
    {
        return data[n];
    }

    [[nodiscard]] friend constexpr auto
    close_to(Entity const& lhs, Entity const& rhs, T tol) -> bool
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
        os << p[0];

        // TODO use `std::views::drop` once clang implements it
        for (auto x : std::ranges::drop_view{p, 1}) {
            os << ", " << x;
        }

        os << ")";
        return os;
    }

    [[nodiscard]] friend constexpr auto
    operator==(Entity const& lhs, Entity const& rhs) -> bool = default;

  private:
    friend Derived<T, N>;

    // Defaulted move constructors are auto noexcept
    // NOLINTBEGIN(performance-noexcept-move-constructor)

    Entity() = default;
    ~Entity() = default;
    Entity(Entity&&) = default;
    Entity(const Entity&) = default;
    auto operator=(Entity&&) -> Entity& = default;
    auto operator=(const Entity&) -> Entity& = default;

    // NOLINTEND(performance-noexcept-move-constructor)

    // Tidy false positive. This constructor is private to allow use by friended
    // types NOLINTBEGIN(modernize-use-equals-delete)

    template <class... Args>
    requires(std::same_as<T, std::remove_cvref_t<Args>>&&...)  //
        explicit(sizeof...(Args) == 1) constexpr Entity(Args&&... xs)
        : data{std::forward<Args>(xs)...}
    {}

    // NOLINTEND(modernize-use-equals-delete)
};

/// A simple vector class
template <class T, std::size_t N>
struct Vector : Entity<Vector<T, N>> {

    Vector() = default;

    template <class... Args>
    requires(std::same_as<T, std::remove_cvref_t<Args>>&&...)  //
        explicit(sizeof...(Args) == 1) constexpr Vector(Args&&... args)
        : Entity<Vector<T, N>>(std::forward<Args>(args)...)
    {}

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

    [[nodiscard]] friend constexpr auto operator*(Vector const& v, T const& s)
        -> Vector
    {
        auto r = Vector{};
        std::transform(v.cbegin(), v.cend(), r.begin(), [s](auto x) {
            return s * x;
        });
        return r;
    }

    [[nodiscard]] friend constexpr auto operator*(T const& s, Vector const& v)
        -> Vector
    {
        return v * s;
    }

    [[nodiscard]] friend constexpr auto norm(Vector const& v)
    {
        return std::inner_product(v.cbegin(), v.cend(), v.cbegin(), T{});
    }
};

/// A simple N-d point class
template <class T, std::size_t N>
struct Point : Entity<Point<T, N>> {
    Point() = default;

    template <class... Args>
    requires(std::same_as<T, std::remove_cvref_t<Args>>&&...)  //
        explicit(sizeof...(Args) == 1) constexpr Point(Args&&... args)
        : Entity<Point>(std::forward<Args>(args)...)
    {}

    [[nodiscard]] friend constexpr auto
    operator+(Point const& p, Vector<T, N> const& v) -> Point
    {
        auto r = Point{};
        std::transform(
            p.cbegin(), p.cend(), v.cbegin(), r.begin(), std::plus{});
        return r;
    }
};

}  // namespace opt
