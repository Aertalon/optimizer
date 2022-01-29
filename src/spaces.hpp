#pragma once

#include "concepts.hpp"
#include "stdx/traits.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <functional>
#include <iostream>
#include <numeric>
#include <ranges>
#include <utility>

namespace opt {

template <class T>
class entity {
    static_assert(stdx::dependent_false<T>,
                  "This primary template should never be instantiated.");
};

template <Arithmetic T,
          std::size_t N,
          template <class, std::size_t>
          class derived>
struct entity<derived<T, N>> {
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
    close_to(const entity& lhs, const entity& rhs, T tol) -> bool
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

    friend auto operator<<(std::ostream& os, const entity& p) -> std::ostream&
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
    operator==(const entity& lhs, const entity& rhs) -> bool = default;

  private:
    friend derived<T, N>;

    // Defaulted move constructors are auto noexcept
    // NOLINTBEGIN(performance-noexcept-move-constructor)

    entity() = default;
    ~entity() = default;
    entity(entity&&) = default;
    entity(const entity&) = default;
    auto operator=(entity&&) -> entity& = default;
    auto operator=(const entity&) -> entity& = default;

    // NOLINTEND(performance-noexcept-move-constructor)

    // Tidy false positive. This constructor is private to allow use by friended
    // types NOLINTBEGIN(modernize-use-equals-delete)

    template <class... Args>  // clang-format off
      requires(std::same_as<T, std::remove_cvref_t<Args>> && ...)
    explicit(sizeof...(Args) == 1) constexpr entity(Args&&... xs)  // clang-format on
        : data{std::forward<Args>(xs)...}
    {}

    // NOLINTEND(modernize-use-equals-delete)
};

/// A simple vector class
template <Arithmetic T, std::size_t N>
struct vector : entity<vector<T, N>> {
    vector() = default;

    template <class... Args>  // clang-format off
      requires(std::same_as<T, std::remove_cvref_t<Args>> && ...)
    explicit(sizeof...(Args) == 1) constexpr vector(Args&&... args)  // clang-format on
        : entity<vector<T, N>>(std::forward<Args>(args)...)
    {}

    [[nodiscard]] friend constexpr auto operator-(const vector& v) -> vector
    {
        auto r = vector{};
        std::transform(v.cbegin(), v.cend(), r.begin(), std::negate{});
        return r;
    }

    [[nodiscard]] friend constexpr auto
    operator+(const vector& v1, const vector& v2) -> vector
    {
        auto r = vector{};
        std::transform(
            v1.cbegin(), v1.cend(), v2.cbegin(), r.begin(), std::plus{});
        return r;
    }

    [[nodiscard]] friend constexpr auto
    operator-(const vector& v1, const vector& v2) -> vector
    {
        return v1 + (-v2);
    }

    [[nodiscard]] friend constexpr auto operator*(const vector& v, const T& s)
        -> vector
    {
        auto r = vector{};
        std::transform(v.cbegin(), v.cend(), r.begin(), [s](auto x) {
            return s * x;
        });
        return r;
    }

    [[nodiscard]] friend constexpr auto operator*(const T& s, const vector& v)
        -> vector
    {
        return v * s;
    }

    [[nodiscard]] friend constexpr auto norm(const vector& v)
    {
        return std::inner_product(v.cbegin(), v.cend(), v.cbegin(), T{});
    }
};

template <class... Ts>
vector(Ts...) -> vector<std::common_type_t<Ts...>, sizeof...(Ts)>;

/// A simple N-d point class
template <Arithmetic T, std::size_t N>
struct point : entity<point<T, N>> {
    point() = default;

    template <class... Args>  // clang-format off
      requires(std::same_as<T, std::remove_cvref_t<Args>> && ...)
    explicit(sizeof...(Args) == 1) constexpr point(Args&&... args)  // clang-format on
        : entity<point>(std::forward<Args>(args)...)
    {}

    [[nodiscard]] friend constexpr auto
    operator-(const point& p, const point& q) -> vector<T, N>
    {
        auto v = vector<T, N>{};
        std::transform(
            p.cbegin(), p.cend(), q.cbegin(), v.begin(), std::minus{});
        return v;
    }

    [[nodiscard]] friend constexpr auto
    operator-(const point& p, const vector<T, N>& v) -> point
    {
        auto r = point{};
        std::transform(
            p.cbegin(), p.cend(), v.cbegin(), r.begin(), std::minus{});
        return r;
    }

    [[nodiscard]] friend constexpr auto
    operator+(const point& p, const vector<T, N>& v) -> point
    {
        auto r = point{};
        std::transform(
            p.cbegin(), p.cend(), v.cbegin(), r.begin(), std::plus{});
        return r;
    }

    [[nodiscard]] friend constexpr auto
    operator+(const vector<T, N>& v, const point& p) -> point
    {
        return p + v;
    }
};

template <class... Ts>
point(Ts...) -> point<std::common_type_t<Ts...>, sizeof...(Ts)>;

}  // namespace opt
