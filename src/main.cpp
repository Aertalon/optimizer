#include "src/convopt/convopt.hpp"
#include "src/dualnumbers/dualnumbers.hpp"

#include <iostream>

class CostMultipleVariables {
  public:
    template <class T>
    constexpr auto operator()(std::array<T, 2> x) const
    {
        // NOLINTBEGIN(readability-magic-numbers)
        return 2.0F * std::get<0>(x) * std::get<0>(x) +
               std::get<0>(x) * (std::get<1>(x) + 7.0F) +
               std::get<1>(x) * std::get<1>(x);
        // NOLINTEND(readability-magic-numbers)
    }
};

auto main() -> int
{
    // FIXME
    // constexpr CostMultipleVariables c{};
    // auto result{convopt::optimize(std::array<float, 2>{10.0F, 20.0F}, c)};
    // auto minval{c(result)};

    // std::cout << "Optimization result: " << result[0] << ", " << result[1]
    //           << "; minimum value: " << minval << std::endl;
    return 0;
}
