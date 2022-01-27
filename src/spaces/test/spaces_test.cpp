#include "src/spaces/spaces.hpp"

#include <gtest/gtest.h>

namespace spaces {

namespace test {}

using vec = Vector<float, 3>;
using point = Point<float, 3>;

void spaces_vector()
{
    constexpr vec zero{};
    constexpr vec v1{1.0F, 0.0F, 0.0F};
    constexpr vec v2{1.0F, 0.0F, -1.0F};

    static_assert(v1.get<0>() == 1.0F, "");
    static_assert(v1.get<1>() == 0.0F, "");
    static_assert(v1.get<2>() == 0.0F, "");

    static_assert(v1 + zero == v1);

    static_assert(-v1 == vec{-1.0F, 0.0F, 0.0F}, "");
    static_assert(v1 + v2 == vec{2.0F, 0.0F, -1.0F}, "");

    static_assert(3.0F * v2 == vec{3.0F, 0.0F, -3.0F}, "");
    static_assert(v2 * 3.0F == vec{3.0F, 0.0F, -3.0F}, "");
}

void spaces_vector_norm()
{
    constexpr vec v{2.0F, 0.0F, -1.0F};
    static_assert(norm(v) == 5.0F, "");
}

void spaces_point()
{
    constexpr point zero{};
    constexpr point p1{1.0F, 0.0F, 0.0F};
    constexpr Vector<float, 3> v{1.0F, 0.0F, 0.0F};

    static_assert(zero + v == p1);
}

}  // namespace spaces
