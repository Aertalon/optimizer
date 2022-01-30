#pragma once

#include "concepts.hpp"
#include "dualnumbers.hpp"
#include "impl/base_fn.hpp"

#include <cmath>
#include <cstddef>
#include <type_traits>

namespace opt {
namespace detail {

inline constexpr impl::base_fn exp = []<Real T>(T x) -> T {
    auto acc = T{1};
    auto power = acc;
    auto factorial = acc;

    constexpr std::size_t taylor_terms{10U};
    for (std::size_t i{1U}; i < taylor_terms; ++i) {
        power *= x;
        factorial *= static_cast<T>(i);

        acc += power / factorial;
    }
    return acc;
};

inline constexpr impl::base_fn cos = []<Real T>(T x) -> T {
    auto acc = T{1};
    auto power = acc;
    auto factorial = acc;

    constexpr std::size_t taylor_terms{10U};
    for (std::size_t i{1U}; i < taylor_terms; ++i) {
        power *= x * x;
        factorial *= static_cast<T>((2 * i - 1) * (2 * i));

        acc += (i % 2 == 0 ? T{1} : -T{1}) * power / factorial;
    }
    return acc;
};

inline constexpr impl::base_fn sin = []<Real T>(T x) -> T {
    auto acc = x;
    auto power = x;
    auto factorial = T{1};

    constexpr std::size_t taylor_terms{10U};
    for (std::size_t i{1U}; i < taylor_terms; ++i) {
        power *= x * x;
        factorial *= static_cast<T>((2 * i) * (2 * i + 1));

        acc += (i % 2 == 0 ? T{1} : -T{1}) * power / factorial;
    }
    return acc;
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

}  // namespace detail

inline constexpr detail::dual_fn exp{detail::exp, detail::exp};
inline constexpr detail::dual_fn sin{detail::sin, detail::cos};
inline constexpr detail::dual_fn cos{detail::cos, -detail::sin};

}  // namespace opt
