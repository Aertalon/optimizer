#pragma once

#include "concepts.hpp"
#include "dualnumbers.hpp"
#include "impl/base_fn.hpp"
#include "impl/series.hpp"

#include <cmath>
#include <cstddef>
#include <iostream>
#include <type_traits>

namespace opt {
namespace impl {

/// A simple binary search algorithm
/// @tparam M The maximum number of iterations of the binary search
/// @tparam T The type of the element to be searched.
/// @tparam F The functional that is zero at the value to be searched.
/// @param low The (initial) lower bound for the value to be searched
/// @param high The (initial) upper bound for the value to be searched
/// @param pred The (monotonic) predicate.
template <std::size_t M, Arithmetic T, class F>
    requires(std::totally_ordered<T> &&
             std::regular_invocable<const F&, const T&>)
constexpr auto binary_search(T low, T high, F const& pred) -> T
{
    std::size_t counter{0};
    while (counter < M) {
        const T mid{(low + high) / (T{1} + T{1})};
        if (pred(mid)) {
            high = mid;
        } else {
            low = mid;
        }
        counter++;
    }
    return low;
}

template <std::size_t N>
inline constexpr auto exp_ = make_base_fn_<N>([]<Real T>(T x) -> T {
    return series::sum_first<N>(series::geometric{
        1U,    //
        T{1},  //
        [x](auto n) { return x / T(n); }});
});

template <std::size_t N>
inline constexpr auto sin_ = make_base_fn_<N>([]<Real T>(T x) -> T {
    return series::sum_first<N>(series::geometric{
        1U,  //
        x,   //
        [x](auto i) {
            const auto n = T(i);
            return (-1) * (x * x) / (2 * n) / (2 * n + 1);
        }});
});

template <std::size_t N>
inline constexpr auto cos_ = make_base_fn_<N>([]<Real T>(T x) -> T {
    return series::sum_first<N>(series::geometric{
        1U,    //
        T{1},  //
        [x](auto i) {
            const auto n = T(i);
            return (-1) * (x * x) / (2 * n - 1) / (2 * n);
        }});
});

template <std::size_t N>
inline constexpr auto sqrt_ = make_base_fn_<N>(
    []<Real T> requires(std::totally_ordered<T>)(T x)->T {
                   const auto lower_than_sqrt = [&x](T const& y) {
                       return x < y * y;
                   };
                   return binary_search<N>(
                       T{0}, x / (T{1} + T{1}) + T{1}, lower_than_sqrt);
               });

template <std::default_initializable F,
          std::default_initializable G,
          std::default_initializable H>
struct dual_fn {
    constexpr dual_fn(F, G, H) noexcept {}

    template <Dual T>
    constexpr auto operator()(const T& x) const -> T
    {
        return {F{}(x.real),
                x.e1 * G{}(x.real),
                x.e2 * G{}(x.real),
                x.e3 * G{}(x.real) + x.e1 * x.e2 * H{}(x.real)};
    }
    template <Real T>
    constexpr auto operator()(T x) const -> T
    {
        return T{F{}(x)};
    }
};

}  // namespace impl

template <std::size_t N>
inline constexpr impl::dual_fn exp_{
    impl::exp_<N>, impl::exp_<N>, impl::exp_<N>};

template <std::size_t N>
inline constexpr impl::dual_fn sin_{
    impl::sin_<N>, impl::cos_<N>, -impl::sin_<N>};

template <std::size_t N>
inline constexpr impl::dual_fn cos_{
    impl::cos_<N>, -impl::sin_<N>, -impl::cos_<N>};

inline constexpr auto exp = exp_<10>;
inline constexpr auto sin = sin_<10>;
inline constexpr auto cos = cos_<10>;

// Only for Real
inline constexpr auto sqrt = impl::sqrt_<30>;

}  // namespace opt
