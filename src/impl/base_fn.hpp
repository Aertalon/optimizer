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

}  // namespace opt::impl
