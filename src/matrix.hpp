#pragma once

#include "spaces.hpp"
#include "spaces_ops.hpp"

namespace opt {

template <Vector V, std::size_t Cols>
struct matrix {
    static constexpr std::size_t rows = std::tuple_size<V>::value;
    static constexpr std::size_t cols = Cols;
    using entries_type = scalar_t<V>;
    using columns_type = V;

    using row_index = typename V::coords_type::size_type;
    using col_index = std::size_t;

    static constexpr bool is_square{V::size == Cols};

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
        row_index row{0};
        col_index col{0};
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

    [[nodiscard]] constexpr auto operator[](col_index n) & -> auto&
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
        return columns[n];
    }
    [[nodiscard]] constexpr auto operator[](col_index n) const& -> auto&
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
        return columns[n];
    }

    [[nodiscard]] constexpr auto
    operator[](std::pair<row_index, col_index> indices) & -> auto&
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
        return columns[indices.second][indices.first];
    }

    [[nodiscard]] constexpr auto
    operator[](std::pair<row_index, col_index> indices) const& -> auto&
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
        return columns[indices.second][indices.first];
    }

    template <row_index R, col_index C>
    [[nodiscard]] constexpr auto get() & -> auto&
    {
        return (std::get<C>(columns)).template get<R>();
    }

    template <row_index R, col_index C>
    [[nodiscard]] constexpr auto get() const& -> auto&
    {
        return (std::get<C>(columns)).template get<R>();
    }

    friend auto operator<<(std::ostream& os, const matrix& p) -> std::ostream&
    {
        os << "[";

        // TODO create a matrix view that traverses the matrix by
        // rows
        for (row_index r{0}; r < matrix::rows; ++r) {
            if (r > 0) {
                os << " ";
            }
            os << p[{r, 0}];
            for (col_index c{1}; c < matrix::cols; ++c) {
                os << ", " << p[{r, c}];
            }
            os << ";" << std::endl;
        }
        os << "]";

        return os;
    }

    [[nodiscard]] friend constexpr auto
    operator==(const matrix& lhs, const matrix& rhs) -> bool = default;

    template <Vector W, std::size_t OtherCols>
    requires(W::size == Cols) [[nodiscard]] friend constexpr auto
    operator*(const matrix<V, Cols>& m0, const matrix<W, OtherCols>& m1)
        -> matrix<V, OtherCols>
    {
        matrix<V, OtherCols> m2{};
        std::transform(
            m1.begin(), m1.end(), m2.begin(), [&m0](const W& m1_column) {
                return m0 * m1_column;
            });
        return m2;
    }

    template <Vector W>
    requires(W::size == Cols) [[nodiscard]] friend constexpr auto
    operator*(const matrix<V, Cols>& m0, const W& v1) -> V
    {
        return std::transform_reduce(
            m0.begin(),
            m0.end(),
            v1.begin(),
            V{},
            std::plus<>{},
            [](const V& m0_column, const scalar_t<V>& s) {
                return s * m0_column;
            });
    }

    [[nodiscard]] friend constexpr auto
    operator+(const matrix<V, Cols>& m0, const matrix<V, Cols>& m1)
        -> matrix<V, Cols>
    {
        matrix<V, Cols> m2{};
        std::transform(
            m0.begin(),
            m0.end(),
            m1.begin(),
            m2.begin(),
            [](const V& m0_col, const V& m1_col) { return m0_col + m1_col; });
        return m2;
    }

    [[nodiscard]] friend constexpr auto
    operator-(const matrix<V, Cols>& m0, const matrix<V, Cols>& m1)
        -> matrix<V, Cols>
    {
        matrix<V, Cols> m2{};
        std::transform(
            m0.begin(),
            m0.end(),
            m1.begin(),
            m2.begin(),
            [](const V& m0_col, const V& m1_col) { return m0_col - m1_col; });
        return m2;
    }
};

}  // namespace opt