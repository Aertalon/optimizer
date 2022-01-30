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

template <int N>
inline constexpr auto exp_ = make_base_fn_<N>([]<Arithmetic T>(T x) -> T {
    return series::sum_first<N>(series::geometric{
        1,     //
        T{1},  //
        [x](auto n) { return x / static_cast<T>(n); }});
});

template <int N>
inline constexpr auto sin_ = make_base_fn_<N>([]<Arithmetic T>(T x) -> T {
    return series::sum_first<N>(series::geometric{
        1,  //
        x,  //
        [x](auto i) {
            const auto n = T(i);
            return (-1) * (x * x) / (2 * n) / (2 * n + 1);
        }});
});

template <int N>
inline constexpr auto cos_ = make_base_fn_<N>([]<Arithmetic T>(T x) -> T {
    return series::sum_first<N>(series::geometric{
        1,     //
        T{1},  //
        [x](auto i) {
            const auto n = T(i);
            return (-1) * (x * x) / (2 * n - 1) / (2 * n);
        }});
});

}  // namespace impl

inline constexpr auto exp = impl::exp_<10>;
inline constexpr auto sin = impl::sin_<10>;
inline constexpr auto cos = impl::cos_<10>;

}  // namespace opt
