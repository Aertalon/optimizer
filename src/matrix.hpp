#pragma once

#include "spaces.hpp"

namespace opt {

struct from_rows {};
struct from_columns {};

template <Arithmetic T, std::size_t Rows, std::size_t Cols>
struct matrix {
    static constexpr std::size_t rows = Rows;
    static constexpr std::size_t cols = Cols;
    using entries_type = T;
    using columns_type = vector<T, Rows>;
    using size_type = std::size_t;

    std::array<vector<T, Rows>, Cols> columns{};

    // Special member functions defaulted, destructor explicit
    ~matrix() = default;
    constexpr matrix() = default;

    // Create matrix from columns
    template <class U, class... Us>  // clang-format off
        requires(
            (std::same_as<T, std::remove_cvref_t<Us>> && ...) && (std::same_as<T, U>) &&
            (sizeof...(Us) == cols-1) &&
            (  (rows != cols) || ((rows == cols) && (rows == 1))  )
            )
    constexpr matrix(
        std::initializer_list<U> const& elem0,
        std::initializer_list<Us> const&... elems)  // clang-format on
        : columns{columns_type{elem0}, columns_type{elems}...}
    {}

    template <class... Args>  // clang-format off
        requires(
            (std::same_as<columns_type, std::remove_cvref_t<Args>> && ...) &&
            (sizeof...(Args) == cols) &&
            (  (rows != cols) || ((rows == cols) && (rows == 1))  )
            )
    constexpr matrix(Args&&... args)  // clang-format on
        : columns{std::forward<Args>(args)...}
    {}

    // Create square matrix from columns
    template <class U, class... Us>  // clang-format off
        requires(
            (std::same_as<T, std::remove_cvref_t<Us>> && ...) && (std::same_as<T, U>) &&
            (sizeof...(Us) == cols-1) && (rows == cols) )
    constexpr matrix(
        from_columns,
        std::initializer_list<U> const& elem0,
        std::initializer_list<Us> const&... elems)  // clang-format on
        : columns{columns_type{elem0}, columns_type{elems}...}
    {}

    template <class... Args>  // clang-format off
        requires(
            (std::same_as<vector<T, cols>, std::remove_cvref_t<Args>> && ...) &&
            (sizeof...(Args) == rows) && (rows == cols))
    constexpr matrix(from_columns, Args&&... args)  // clang-format on
        : columns{std::forward<Args>(args)...}
    {}

    // Create matrix from rows
    template <class U, class... Us>  // clang-format off
        requires(
            (std::same_as<T, std::remove_cvref_t<Us>> && ...) && (std::same_as<T, U>) &&
            (sizeof...(Us) == rows-1) && (rows != cols) )
    constexpr matrix(
        std::initializer_list<U> const& elem0,
        std::initializer_list<Us> const&... elems)  // clang-format on
    {
        *this = matrix<T, cols, rows>{elem0, elems...}.transpose();
    }

    template <class... Args>  // clang-format off
        requires(
            (std::same_as<vector<T, cols>, std::remove_cvref_t<Args>> && ...) &&
            (sizeof...(Args) == rows) && (rows != cols))
    constexpr matrix(Args&&... args)  // clang-format on
    {
        *this = matrix<T, cols, rows>{args...}.transpose();
    }

    // Create square matrix from rows
    template <class U, class... Us>  // clang-format off
        requires(
            (std::same_as<T, std::remove_cvref_t<Us>> && ...) && (std::same_as<T, U>) &&
            (sizeof...(Us) == rows-1) && (rows == cols) )
    constexpr matrix(
        from_rows,
        std::initializer_list<U> const& elem0,
        std::initializer_list<Us> const&... elems)  // clang-format on
    {
        *this =
            matrix<T, cols, rows>{from_columns{}, elem0, elems...}.transpose();
    }

    template <class... Args>  // clang-format off
        requires(
            (std::same_as<vector<T, cols>, std::remove_cvref_t<Args>> && ...) &&
            (sizeof...(Args) == rows) && (rows == cols))
    constexpr matrix(from_rows, Args&&... args)  // clang-format on
    {
        *this = matrix<T, cols, rows>{from_columns{}, args...}.transpose();
    }

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

    [[nodiscard]] constexpr auto
    operator[](typename columns_type::coords_type::size_type n) & -> auto&
    {
        return columns[n];
    }
    [[nodiscard]] constexpr auto
    operator[](typename columns_type::coords_type::size_type n) const& -> auto&
    {
        return columns[n];
    }

    [[nodiscard]] constexpr auto
    operator[](std::pair<typename columns_type::coords_type::size_type,
                         typename matrix::size_type> indices) & -> auto&
    {
        return columns[indices.second][indices.first];
    }
    [[nodiscard]] constexpr auto
    operator[](std::pair<typename columns_type::coords_type::size_type,
                         typename matrix::size_type> indices) const& -> auto&
    {
        return columns[indices.second][indices.first];
    }

    friend auto operator<<(std::ostream& os, const matrix& p) -> std::ostream&
    {
        os << "[" << std::endl;

        // TODO create a matrix view that traverses the matrix by
        // rows
        for (matrix::size_type r{0}; r < matrix::rows; ++r) {
            os << p[{r, 0}];
            for (std::size_t c{1}; c < matrix::cols; ++c) {
                os << ", " << p[{r, c}];
            }
            os << ";" << std::endl;
        }
        os << "]";

        return os;
    }

    [[nodiscard]] friend constexpr auto
    operator==(const matrix& lhs, const matrix& rhs) -> bool = default;

    [[nodiscard]] constexpr auto transpose() const -> matrix<T, Cols, Rows>
    {
        matrix<T, Cols, Rows> t{};

        // FIXME: clean up this mess
        for (std::size_t r{0}; r < matrix::rows; ++r) {
            for (std::size_t c{0}; c < matrix::cols; ++c) {
                t[{c, r}] = (*this)[{r, c}];
            }
        }
        return t;
    }

    template <std::size_t OtherCols>
    [[nodiscard]] friend constexpr auto
    operator*(const matrix<T, Rows, Cols>& m0,
              const matrix<T, Cols, OtherCols>& m1)
        -> matrix<T, Rows, OtherCols>
    {
        matrix<T, Rows, OtherCols> m2{};
        std::transform(
            m1.begin(),
            m1.end(),
            m2.begin(),
            [&m0](const vector<T, Cols>& m1_column) {
                return std::transform_reduce(
                    m0.begin(),
                    m0.end(),
                    m1_column.begin(),
                    vector<T, Rows>{},
                    std::plus<>{},
                    [](const vector<T, Rows>& m0_column, const T& s) {
                        return s * m0_column;
                    });
            });
        return m2;
    }

    [[nodiscard]] friend constexpr auto
    operator+(const matrix<T, Rows, Cols>& m0, const matrix<T, Rows, Cols>& m1)
        -> matrix<T, Rows, Cols>
    {
        matrix<T, Rows, Cols> m2{};
        std::transform(
            m0.begin(),
            m0.end(),
            m1.begin(),
            m2.begin(),
            [](const vector<T, Rows>& m0_col, const vector<T, Rows>& m1_col) {
                return m0_col + m1_col;
            });
        return m2;
    }

    [[nodiscard]] friend constexpr auto
    operator-(const matrix<T, Rows, Cols>& m0, const matrix<T, Rows, Cols>& m1)
        -> matrix<T, Rows, Cols>
    {
        matrix<T, Rows, Cols> m2{};
        std::transform(
            m0.begin(),
            m0.end(),
            m1.begin(),
            m2.begin(),
            [](const vector<T, Rows>& m0_col, const vector<T, Rows>& m1_col) {
                return m0_col - m1_col;
            });
        return m2;
    }
};

// FIXME: this should be a matrix method
template <class T, std::size_t Rows, std::size_t Cols>
requires(Rows == Cols)
    [[nodiscard]] constexpr auto trace(matrix<T, Rows, Cols> const& m) -> T
{
    // FIXME: hideous stuff here
    const auto indices = std::ranges::iota_view{0U, Rows};
    return std::transform_reduce(
        m.begin(),
        m.end(),
        indices.begin(),
        T{},
        std::plus{},
        [](const vector<T, Rows>& m_col, const std::size_t i) {
            return m_col[i];
        });
}

namespace detail {
template <class T, std::size_t Dim, std::size_t... Is>
constexpr auto make_identity_impl(std::index_sequence<Is...>)
    -> matrix<T, Dim, Dim>
{
    return matrix<T, Dim, Dim>{from_columns{}, canonical_vector<T, Dim, Is>...};
}

template <class T, std::size_t Dim>
constexpr auto make_identity() -> matrix<T, Dim, Dim>
{
    return make_identity_impl<T, Dim>(std::make_index_sequence<Dim>{});
}
}  // namespace detail

template <class T, std::size_t Dim>
inline constexpr auto identity = detail::make_identity<T, Dim>();

}  // namespace opt