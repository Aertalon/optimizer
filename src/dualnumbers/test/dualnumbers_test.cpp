#include "src/dualnumbers/dualnumbers.hpp"

#include <gtest/gtest.h>

namespace opt {

namespace test {}

void dualnumbers_sum()
{
    constexpr DualNumber x{1.0F, 0.0F};
    constexpr DualNumber y{0.0F, 1.0F};

    static_assert(x + y == DualNumber{1.0F, 1.0F});
    static_assert(y + x == DualNumber{1.0F, 1.0F});
}

void dualnumbers_product()
{
    constexpr DualNumber x{1.0F, 0.0F};
    constexpr DualNumber y{0.0F, 1.0F};

    static_assert(x * y == DualNumber{0.0F, 1.0F});
    static_assert(y * x == DualNumber{0.0F, 1.0F});
}

// Add tests for / and affine and nonlinear functions

}  // namespace opt
