#pragma once

#include <concepts>
#include <utility>

namespace opt::impl {

template <std::default_initializable F>
struct base_fn {
    base_fn() = default;
    constexpr base_fn(F) noexcept {}

    template <class... Args>
    constexpr auto operator()(Args&&... args) const
        -> decltype(F{}(std::forward<Args>(args)...))
    {
        return F{}(std::forward<Args>(args)...);
    }

    constexpr auto operator-() const
    {
        return  // clang-format off
        []<class... Ts>(Ts&&... args)
        -> decltype(-F{}(std::forward<Ts...>(args)...))
        {
          return -F{}(std::forward<Ts...>(args)...);
        };  // clang-format on
    }
};

template <std::size_t N, std::default_initializable F>
struct base_fn_n : base_fn<F> {
    static constexpr auto count = N;

    using base_fn<F>::base_fn;
};

template <std::size_t N, class F>
constexpr auto make_base_fn_(F) -> base_fn_n<N, F>
{
    return {};
}

}  // namespace opt::impl
