#pragma once

#include <concepts>
#include <cstddef>
#include <memory>
#include <optional>
#include <ranges>
#include <type_traits>

namespace opt::impl::series {

template <class T>
// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
struct copyable_wrapper : std::optional<T> {
    // NOLINTNEXTLINE(modernize-use-equals-default)
    using std::optional<T>::optional;

    constexpr copyable_wrapper()
        requires std::default_initializable<T>
    : std::optional<T>{std::in_place}
    {}

    constexpr copyable_wrapper(const copyable_wrapper&) = default;

    constexpr auto
    operator=(const copyable_wrapper& other) & -> copyable_wrapper&
    {
        if (this != std::addressof(other)) {
            if (other) {
                emplace(*other);
            } else {
                this->reset();
            }
        }
        return *this;
    }
};

template <std::regular_invocable<std::size_t> F,
          Arithmetic R = std::invoke_result_t<const F&, std::size_t>>
struct geometric : std::ranges::view_interface<geometric<F>> {
    copyable_wrapper<F> func{};
    std::size_t step{};
    R acc{};

    geometric() = default;

    template <class T>
        requires std::same_as<F, std::remove_cvref_t<T>>
    // NOLINTNEXTLINE(bugprone-forwarding-reference-overload)
    constexpr geometric(T&& t) : func{std::forward<T>(t)}, acc{1}
    {}

    template <std::unsigned_integral I, std::same_as<R> A, class T>
        requires std::same_as<F, std::remove_cvref_t<T>>
    constexpr geometric(I n0, A init, T&& t)
        : func{std::forward<T>(t)}, step{n0}, acc{init}
    {}

    struct iterator {
        geometric* parent;

        using value_type = R;
        using pointer = R*;
        using reference = const R&;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::input_iterator_tag;

        [[nodiscard]] constexpr auto operator*() const -> const R&
        {
            return parent->acc;
        }

        constexpr auto operator++() -> iterator&
        {
            parent->acc *= (*parent->func)(parent->step++);
            return *this;
        }
        [[nodiscard]] constexpr auto operator++(int) -> iterator
        {
            auto tmp = *this;
            ++*this;
            return tmp;
        }
    };

    [[nodiscard]] constexpr auto begin() -> iterator { return {this}; }
    [[nodiscard]] constexpr auto end() const -> std::unreachable_sentinel_t
    {
        return {};
    }
};

template <class F>
geometric(F) -> geometric<std::remove_cvref_t<F>>;
template <class F>
geometric(std::size_t n, std::invoke_result_t<const F&, std::size_t>, F)
    -> geometric<std::remove_cvref_t<F>>;

template <std::size_t N,
          std::ranges::input_range R,
          Arithmetic T = std::remove_cvref_t<std::ranges::range_value_t<R>>>
constexpr auto sum_first(R&& rng, T init = {})
{
    for (const auto& x : std::ranges::take_view{std::forward<R>(rng), N}) {
        init += x;
    }
    return init;
}

}  // namespace opt::impl::series

template <class F, class R>
inline constexpr bool
    std::ranges::enable_borrowed_range<opt::impl::series::geometric<F, R>> =
        true;
