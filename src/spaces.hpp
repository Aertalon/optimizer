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

namespace detail {
struct close_to_fn {
  private:
    template <class T1, class T2, class S>
    [[nodiscard]] constexpr auto
    impl(T1&& lhs, T2&& rhs, S&& tol, stdx::priority_tag_t<1>) const noexcept(
        noexcept(close_to(std::forward<T1>(lhs),
                          std::forward<T2>(rhs),
                          std::forward<S>(tol))))
        -> decltype(close_to(
            std::forward<T1>(lhs), std::forward<T2>(rhs), std::forward<S>(tol)))
    {
        return close_to(
            std::forward<T1>(lhs), std::forward<T2>(rhs), std::forward<S>(tol));
    }
    template <class T>  // clang-format off
      requires(Point<T> || Vector<T>)
    [[nodiscard]] constexpr auto  // clang-format on
        impl(T const& lhs,
             T const& rhs,
             scalar_t<T> tol,
             stdx::priority_tag_t<0>) const -> bool
    {
        // TODO move to common location or replace with constexpr math lib
        constexpr auto abs = [](auto x) {
            if (x < decltype(x){}) {
                return -x;
            }

            return x;
        };

        return [&lhs, &rhs, &tol,
                abs ]<std::size_t... Is>(std::index_sequence<Is...>)
        {
            return ((abs(lhs[Is] - rhs[Is]) < tol) && ...);
        }
        (std::make_index_sequence<std::tuple_size_v<T>>{});
    }

  public:
    template <class T1, class T2, class S>
    [[nodiscard]] constexpr auto
    operator()(T1&& lhs, T2&& rhs, S&& tol) const noexcept(noexcept(impl(
        std::forward<T1>(lhs),
        std::forward<T2>(rhs),
        std::forward<S>(tol),
        stdx::priority_tag<1>)))
        -> decltype(impl(std::forward<T1>(lhs),
                         std::forward<T2>(rhs),
                         std::forward<S>(tol),
                         stdx::priority_tag<1>))
    {
        return impl(std::forward<T1>(lhs),
                    std::forward<T2>(rhs),
                    std::forward<S>(tol),
                    stdx::priority_tag<1>);
    }
};
}  // namespace detail
inline constexpr detail::close_to_fn close_to{};

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

    using entries_types = T;
    using coords_type = std::array<T, N>;
    using index_type = std::size_t;

    static constexpr std::size_t size = N;

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

    template <typename coords_type::size_type Idx>
        requires(Idx < N) [
                [nodiscard]] constexpr auto get() & -> auto&
    {
        return std::get<Idx>(data);
    }

    template <typename coords_type::size_type Idx>
        requires(Idx < N) [
                [nodiscard]] constexpr auto get() const& -> auto&
    {
        return std::get<Idx>(data);
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

    explicit constexpr entity(std::initializer_list<T> list)  // clang-format on
    {
        std::copy(list.begin(), list.end(), data.begin());
    }

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

    explicit constexpr vector(std::initializer_list<T> list)  // clang-format on
        : entity<vector<T, N>>(std::forward<std::initializer_list<T>>(list))
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
        auto r = vector{v};
        r *= s;
        return r;
    }

    [[nodiscard]] friend constexpr auto operator*(const T& s, const vector& v)
        -> vector
    {
        return v * s;
    }

    constexpr auto operator*=(const T& s) -> vector&
    {
        std::transform(
            (*this).cbegin(), (*this).cend(), (*this).begin(), [s](auto x) {
                return s * x;
            });
        return *this;
    }
};

template <class... Ts>
vector(Ts...) -> vector<std::common_type_t<Ts...>, sizeof...(Ts)>;

template <class, std::size_t>
struct extend_to {};

template <Arithmetic T, std::size_t N, std::size_t NewN>
struct extend_to<vector<T, N>, NewN> {
    using type = vector<T, NewN>;
};

template <Vector V, std::size_t NewN>
using extend_to_t = typename extend_to<V, NewN>::type;

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

template <class T, std::size_t Size>
struct std::tuple_size<opt::vector<T, Size>>
    : std::integral_constant<std::size_t, Size> {};

template <class T, std::size_t Size>
struct std::tuple_size<opt::point<T, Size>>
    : std::integral_constant<std::size_t, Size> {};
