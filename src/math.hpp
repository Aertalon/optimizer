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

inline constexpr base_fn exp = []<Real T>(T x) -> T {
    return series::sum_first<10>(series::geometric{
        1U,    //
        T{1},  //
        [x](auto n) { return x / T(n); }});
};

inline constexpr impl::base_fn sin = []<Real T>(T x) -> T {
    return series::sum_first<10>(series::geometric{
        1U,  //
        x,   //
        [x](auto i) {
            const auto n = T(i);
            return (-1) * (x * x) / (2 * n) / (2 * n + 1);
        }});
};

inline constexpr impl::base_fn cos = []<Real T>(T x) -> T {
    return series::sum_first<10>(series::geometric{
        1U,    //
        T{1},  //
        [x](auto i) {
            const auto n = T(i);
            return (-1) * (x * x) / (2 * n - 1) / (2 * n);
        }});
};

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

inline constexpr impl::dual_fn exp{impl::exp, impl::exp};
inline constexpr impl::dual_fn sin{impl::sin, impl::cos};
inline constexpr impl::dual_fn cos{impl::cos, -impl::sin};

}  // namespace opt
