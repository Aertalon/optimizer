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

    constexpr coords_type get_coords() const { return coords_; }

    template <std::size_t I>
    constexpr coord_type get() const
    {
        return std::get<I>(coords_);
    }

  private:
    template <std::size_t... Is>
    friend constexpr bool close_to_impl(
        Entity const& lhs,
        Entity const& rhs,
        coord_type tol,
        std::index_sequence<Is...>)
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

    friend constexpr bool
    close_to(Entity const& lhs, Entity const& rhs, coord_type tol)
    {
        return close_to_impl(lhs, rhs, tol, std::make_index_sequence<size>{});
    }

    template <std::size_t... Is>
    friend std::ostream& operator_stream_impl(
        std::ostream& os, Entity const& p, std::index_sequence<Is...>)
    {
        using unused = int[];
        os << "(";

        static_cast<void>(
            unused{0, (os << std::get<Is>(p.coords_) << ",", 0)...});
        os << ")";

        return os;
    }

    friend std::ostream& operator<<(std::ostream& os, Entity const& p)
    {
        return operator_stream_impl(os, p, std::make_index_sequence<size>{});
    }

    template <std::size_t... Is>
    friend constexpr bool operator_equal_impl(
        Entity const& lhs, Entity const& rhs, std::index_sequence<Is...>)
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

    friend constexpr bool operator==(Entity const& lhs, Entity const& rhs)
    {
        return operator_equal_impl(lhs, rhs, std::make_index_sequence<size>{});
    }

    friend constexpr bool operator!=(Entity const& lhs, Entity const& rhs)
    {
        return !(lhs == rhs);
    }

    template <class... Args>
    constexpr explicit Entity(coord_type x, Args&&... xs)
    {
        coords_ = {x, xs...};
    }

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
    friend constexpr Vector
    operator_unary_negate_impl(Vector const& v, std::index_sequence<Is...>)
    {
        return {-std::get<Is>(v.get_coords())...};
    }

    friend constexpr Vector operator-(Vector const& v)
    {
        return operator_unary_negate_impl(
            v, std::make_index_sequence<Vector::size>{});
    }

    template <std::size_t... Is>
    friend constexpr Vector operator_binary_sum_impl(
        Vector const& v1, Vector const& v2, std::index_sequence<Is...>)
    {
        auto const coords_v1{v1.get_coords()};
        auto const coords_v2{v2.get_coords()};

        return {(std::get<Is>(coords_v1) + std::get<Is>(coords_v2))...};
    }

    friend constexpr Vector operator+(Vector const& v1, Vector const& v2)
    {
        return operator_binary_sum_impl(
            v1, v2, std::make_index_sequence<Vector::size>{});
    }

    template <std::size_t... Is>
    friend constexpr Vector operator_scalar_prod_impl(
        Vector const& v,
        Vector::coord_type const& s,
        std::index_sequence<Is...>)
    {
        auto const coords_v{v.get_coords()};

        return {(s * std::get<Is>(coords_v))...};
    }

    friend constexpr Vector
    operator*(Vector const& v, Vector::coord_type const& s)
    {
        return operator_scalar_prod_impl(
            v, s, std::make_index_sequence<Vector::size>{});
    }

    friend constexpr Vector
    operator*(Vector::coord_type const& s, Vector const& v)
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
    friend constexpr Point operator_vector_point_sum_impl(
        Point const& p,
        Vector<VecType, VecSize> const& v,
        std::index_sequence<Is...>)
    {
        auto const coords_p{p.get_coords()};
        auto const coords_v{v.get_coords()};

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
