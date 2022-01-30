#pragma once

#include "concepts.hpp"
#include "dualnumbers.hpp"
#include "impl/base_fn.hpp"
#include "impl/series.hpp"

#include <cmath>
#include <cstddef>
#include <type_traits>

namespace opt {
namespace impl {

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

template <std::default_initializable F, std::default_initializable G>
struct dual_fn {
    constexpr dual_fn(F, G) noexcept {}

    template <Dual T>
    constexpr auto operator()(const T& x) const -> T
    {
        return {F{}(x.real), x.imag * G{}(x.real)};
    }
    template <Real T>
    constexpr auto operator()(T x) const -> T
    {
        return T{F{}(x)};
    }
};

}  // namespace impl

template <std::size_t N>
inline constexpr impl::dual_fn exp_{impl::exp_<N>, impl::exp_<N>};

template <std::size_t N>
inline constexpr impl::dual_fn sin_{impl::sin_<N>, impl::cos_<N>};

template <std::size_t N>
inline constexpr impl::dual_fn cos_{impl::cos_<N>, -impl::sin_<N>};

inline constexpr auto exp = exp_<10>;
inline constexpr auto sin = sin_<10>;
inline constexpr auto cos = cos_<10>;

}  // namespace opt
