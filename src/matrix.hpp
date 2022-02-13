#pragma once

#include "spaces.hpp"
#include "spaces_ops.hpp"

#include <utility>

namespace opt {

template <class T, std::size_t N>
consteval auto make_sequence_of()
{
    return []<std::size_t... Is>(std::index_sequence<Is...>)
    {
        return std::integer_sequence<T, T{Is}...>{};
    }
    (std::make_index_sequence<N>{});
}

template <Vector V, std::size_t Cols>
struct matrix {
    static constexpr std::size_t rows = std::tuple_size<V>::value;
    static constexpr std::size_t cols = Cols;
    using entries_type = scalar_t<V>;
    using columns_type = V;

    using row_index_type = typename columns_type::coords_type::size_type;
    using col_index_type = std::size_t;

    template <row_index_type... Rs>
    using row_sequence = std::integer_sequence<row_index_type, Rs...>;
    template <col_index_type... Cs>
    using col_sequence = std::integer_sequence<col_index_type, Cs...>;

    struct entry {
        row_index_type row;
        col_index_type col;
    };

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
        row_index_type row{0};
        col_index_type col{0};
        (
            [&row, &col, &the_columns = columns](auto&& elem) {
                // FIXME use index sequences
                // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
                the_columns[col][row] = elem;
                col = (col == cols - col_index_type{1}) ?
                          col_index_type{0} :
                          col + col_index_type{1};
                row =
                    (col == col_index_type{0}) ? row + row_index_type{1} : row;
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

    [[nodiscard]] constexpr auto operator[](col_index_type n) & -> auto&
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
        return columns[n];
    }
    [[nodiscard]] constexpr auto operator[](col_index_type n) const& -> auto&
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
        return columns[n];
    }

    [[nodiscard]] constexpr auto operator[](entry const& indices) & -> auto&
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
        return columns[indices.col][indices.row];
    }

    [[nodiscard]] constexpr auto
    operator[](entry const& indices) const& -> auto&
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
        return columns[indices.col][indices.row];
    }

    template <row_index_type R, col_index_type C>
    [[nodiscard]] constexpr auto get() & -> auto&
    {
        return (std::get<C>(columns)).template get<R>();
    }

    template <row_index_type R, col_index_type C>
    [[nodiscard]] constexpr auto get() const& -> auto&
    {
        return (std::get<C>(columns)).template get<R>();
    }

    friend auto operator<<(std::ostream& os, const matrix& p) -> std::ostream&
    {
        os << "[";

        // TODO create a matrix view that traverses the matrix by
        // rows
        for (row_index_type r{0}; r < matrix::rows; ++r) {
            if (r > 0) {
                os << " ";
            }
            os << p[{r, 0}];
            for (col_index_type c{1}; c < matrix::cols; ++c) {
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
        requires(W::size == Cols) [
                [nodiscard]] friend constexpr auto
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
        requires(W::size == Cols) [
                [nodiscard]] friend constexpr auto
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

    [[nodiscard]] constexpr auto transpose() const
        -> matrix<extend_to_t<V, cols>, rows>
    {
        return [&self = *this]() {
            matrix<extend_to_t<V, cols>, rows> t{};
            [&self, &t ]<row_index_type... Rs>(row_sequence<Rs...>)
            {
                // Cycle over rows
                (
                    [&self, &t ]<row_index_type R, col_index_type... Cs>(
                        // Given row R, cycle over columns
                        col_sequence<Cs...>,
                        row_sequence<R>) {
                        ((t.template get<Cs, R>() = self.template get<R, Cs>()),
                         ...);
                    }(make_sequence_of<col_index_type, cols>(),
                      row_sequence<Rs>{}),
                    ...);
            }
            (make_sequence_of<row_index_type, rows>());
            return t;
        }();
    }
};

}  // namespace opt