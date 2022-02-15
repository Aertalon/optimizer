#pragma once

#include "spaces.hpp"

namespace opt {

template <Vector V, std::size_t Cols>
struct matrix {
    static constexpr std::size_t rows = std::tuple_size<V>::value;
    static constexpr std::size_t cols = Cols;
    using entries_type = scalar_t<V>;
    using columns_type = V;

    std::array<V, Cols> columns{};

    // Special member functions defaulted
    constexpr matrix() = default;

    // Ctor from elements, row major
    template <class... Us>  // clang-format off
        requires(
            (std::same_as<entries_type, std::remove_cvref_t<Us>> && ...) &&
            (sizeof...(Us) == (cols * rows)))
    constexpr matrix(Us&&... us)  // clang-format on
    {
        std::size_t row{0};
        std::size_t col{0};
        (
            [&row, &col, &the_columns = columns](auto&& elem) {
                // FIXME use index sequences
                // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
                the_columns[col][row] = elem;
                col = (col == cols - 1) ? 0 : col + 1;
                row = (col == 0) ? row + 1 : row;
            }(std::forward<Us>(us)),
            ...);
    }

    // Ctor from Vectors
    template <class... Us>  // clang-format off
        requires(
            (std::same_as<columns_type, std::remove_cvref_t<Us>> && ...) &&
            (sizeof...(Us) == cols))
    constexpr matrix(Us&&... us)  // clang-format on
        : columns{std::forward<Us>(us)...}
    {}

    // Ctor from initialization list into Vectors
    template <class U, class... Us>  // clang-format off
        requires(
            (std::same_as<entries_type, std::remove_cvref_t<Us>> && ...) && (std::same_as<entries_type, U>) &&
            (sizeof...(Us) == cols-1))
    constexpr matrix(
        std::initializer_list<U> const& elem0,
        std::initializer_list<Us> const&... elems)  // clang-format on
        : columns{columns_type{elem0}, columns_type{elems}...}
    {}

    [[nodiscard]] constexpr auto begin() & noexcept { return columns.begin(); }
    [[nodiscard]] constexpr auto begin() const& noexcept
    {
        return columns.begin();
    }
    [[nodiscard]] constexpr auto cbegin() const& noexcept
    {
        return columns.cbegin();
    }

    [[nodiscard]] constexpr auto end() & noexcept { return columns.end(); }
    [[nodiscard]] constexpr auto end() const& noexcept { return columns.end(); }
    [[nodiscard]] constexpr auto cend() const& noexcept
    {
        return columns.cend();
    }

    [[nodiscard]] friend constexpr auto
    operator==(const matrix& lhs, const matrix& rhs) -> bool = default;
};

}  // namespace opt