#include "src/convopt/convopt.hpp"
#include "src/dualnumbers/math.hpp"
#include "src/spaces/spaces.hpp"

#include <gtest/gtest.h>

namespace convopt {

namespace test {}

class Cost {
  public:
    template <class T>
    constexpr T operator()(Point<T, 2> const& x) const
    {
        auto x_part{x.template get<0>() - T{3.0F}};
        auto y_part{x.template get<1>() - T{-1.0F}};

        return dualnumbers::exp(x_part * x_part) + y_part * y_part;
    }
};

void convopt_gradient()
{
    constexpr Cost cost{};
    constexpr Point<float, 2> p{1.0F, 0.0F};
    static_assert(gradient(p, cost) ==
                      Vector<float, 2>{-4.0F * dualnumbers::exp(4.0F), 2.0F},
                  "");
}

void convopt_optimize()
{
    constexpr Cost cost{};
    constexpr Point<float, 2> p{1.0F, 0.0F};

    static_assert(
        close_to(optimize(p, cost), Point<float, 2>{3.0F, -1.0F}, 1e-3F), "");
}

// Add tests

}  // namespace convopt
