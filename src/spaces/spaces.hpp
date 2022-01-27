#pragma once

#include <array>
#include <iostream>
#include <utility>

namespace spaces {

template <class T>
class Entity {};

template <std::size_t N, class T, template <class, std::size_t> class Derived>
class Entity<Derived<T, N>> {
  public:
    static constexpr std::size_t size{N};
    using coord_type = T;
    using coords_type = std::array<coord_type, size>;

    [[nodiscard]] constexpr auto coords() const -> coords_type
    {
        return coords_;
    }

    template <std::size_t I>
    [[nodiscard]] constexpr auto get() const -> coord_type
    {
        return std::get<I>(coords_);
    }

  private:
    template <std::size_t... Is>
    friend constexpr auto close_to_impl(
        Entity const& lhs,
        Entity const& rhs,
        coord_type tol,
        std::index_sequence<Is...>) -> bool
    {
        using unused = int[];
        bool result{true};

        static_cast<void>(unused{
            0,
            (result =
                 result &&
                 (std::get<Is>(lhs.coords_) - std::get<Is>(rhs.coords_) >
                  -tol) &&
                 (std::get<Is>(lhs.coords_) - std::get<Is>(rhs.coords_) < tol),
             0)...});

        return result;
    }

    friend constexpr auto
    close_to(Entity const& lhs, Entity const& rhs, coord_type tol) -> bool
    {
        return close_to_impl(lhs, rhs, tol, std::make_index_sequence<size>{});
    }

    template <std::size_t... Is>
    friend auto operator_stream_impl(
        std::ostream& os, Entity const& p, std::index_sequence<Is...>)
        -> std::ostream&
    {
        using unused = int[];
        os << "(";

        static_cast<void>(
            unused{0, (os << std::get<Is>(p.coords_) << ",", 0)...});
        os << ")";

        return os;
    }

    friend auto operator<<(std::ostream& os, Entity const& p) -> std::ostream&
    {
        return operator_stream_impl(os, p, std::make_index_sequence<size>{});
    }

    template <std::size_t... Is>
    friend constexpr auto operator_equal_impl(
        Entity const& lhs, Entity const& rhs, std::index_sequence<Is...>)
        -> bool
    {
        using unused = int[];
        bool result{true};

        static_cast<void>(unused{
            0,
            (result = result &&
                      (std::get<Is>(lhs.coords_) == std::get<Is>(rhs.coords_)),
             0)...});

        return result;
    }

    friend constexpr auto operator==(Entity const& lhs, Entity const& rhs)
        -> bool
    {
        return operator_equal_impl(lhs, rhs, std::make_index_sequence<size>{});
    }

    friend constexpr auto operator!=(Entity const& lhs, Entity const& rhs)
        -> bool
    {
        return !(lhs == rhs);
    }

    template <class... Args>
    constexpr explicit Entity(coord_type x, Args&&... xs) : coords_({x, xs...})
    {}

    constexpr Entity() = default;

    constexpr Entity(coords_type coords) : coords_{std::move(coords)} {}

    friend Derived<T, N>;
    coords_type coords_{};
};

/// A simple vector class
template <class T, std::size_t N>
class Vector : public Entity<Vector<T, N>> {
  public:
    static constexpr std::size_t size = N;
    using coord_type = typename Entity<Vector<T, N>>::coord_type;

    template <class... Args>
    constexpr Vector(Args&&... args)
        : Entity<Vector<T, N>>(std::forward<Args>(args)...)
    {}

  private:
    template <std::size_t... Is>
    friend constexpr auto
    operator_unary_negate_impl(Vector const& v, std::index_sequence<Is...>)
        -> Vector
    {
        return {-std::get<Is>(v.coords())...};
    }

    friend constexpr auto operator-(Vector const& v) -> Vector
    {
        return operator_unary_negate_impl(
            v, std::make_index_sequence<Vector::size>{});
    }

    template <std::size_t... Is>
    friend constexpr auto operator_binary_sum_impl(
        Vector const& v1, Vector const& v2, std::index_sequence<Is...>)
        -> Vector
    {
        auto const coords_v1{v1.coords()};
        auto const coords_v2{v2.coords()};

        return {(std::get<Is>(coords_v1) + std::get<Is>(coords_v2))...};
    }

    friend constexpr auto operator+(Vector const& v1, Vector const& v2)
        -> Vector
    {
        return operator_binary_sum_impl(
            v1, v2, std::make_index_sequence<Vector::size>{});
    }

    template <std::size_t... Is>
    friend constexpr auto operator_scalar_prod_impl(
        Vector const& v,
        Vector::coord_type const& s,
        std::index_sequence<Is...>) -> Vector
    {
        auto const coords_v{v.coords()};

        return {(s * std::get<Is>(coords_v))...};
    }

    friend constexpr auto
    operator*(Vector const& v, Vector::coord_type const& s) -> Vector
    {
        return operator_scalar_prod_impl(
            v, s, std::make_index_sequence<Vector::size>{});
    }

    friend constexpr auto
    operator*(Vector::coord_type const& s, Vector const& v) -> Vector
    {
        return v * s;
    }

    template <std::size_t... Is>
    friend constexpr auto norm_impl(Vector const& v, std::index_sequence<Is...>)
    {
        using unused = int[];
        Vector::coord_type ret{};
        static_cast<void>(unused{
            0, (ret += v.template get<Is>() * v.template get<Is>(), 0)...});

        return ret;
    }

    friend constexpr auto norm(Vector const& v)
    {
        return norm_impl(v, std::make_index_sequence<Vector::size>{});
    }
};

/// A simple 3-d point class
template <class T, std::size_t N>
class Point : public Entity<Point<T, N>> {
  public:
    static constexpr std::size_t size = N;

    template <class... Args>
    constexpr Point(Args&&... args) : Entity<Point>(std::forward<Args>(args)...)
    {}

  private:
    template <class VecType, std::size_t VecSize, std::size_t... Is>
    friend constexpr auto operator_vector_point_sum_impl(
        Point const& p,
        Vector<VecType, VecSize> const& v,
        std::index_sequence<Is...>) -> Point
    {
        auto const coords_p{p.coords()};
        auto const coords_v{v.coords()};

        return {(std::get<Is>(coords_p) + std::get<Is>(coords_v))...};
    }

    template <class VecType, std::size_t VecSize>
    friend constexpr auto
    operator+(Point const& p, Vector<VecType, VecSize> const& v)
        -> std::enable_if_t<
            std::is_same<typename Point::coord_type, VecType>::value &&
                (Point::size == VecSize),
            Point>
    {
        return operator_vector_point_sum_impl(
            p, v, std::make_index_sequence<Point::size>{});
    }
};

}  // namespace spaces
