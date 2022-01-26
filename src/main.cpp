#include <iostream>

#include "src/dualnumbers/dualnumbers.h"
#include "src/convopt/convopt.h"

class CostMultipleVariables
{
    public:
        template<class T>
        constexpr auto operator()(std::array<T,2> x) const
        {
          return 2.0F*std::get<0>(x)*std::get<0>(x) + std::get<0>(x) * (std::get<1>(x) + 7.0F) + std::get<1>(x)*std::get<1>(x);
        }
};

int main()
{
    constexpr CostMultipleVariables c{};
    auto result{convopt::optimize(std::array<float, 2>{10.0F, 20.0F}, c)};
    auto minval{c(result)};

    std::cout << "Optimization result: " << result[0] << ", " << result[1] << "; minimum value: " << minval << std::endl;
    return 0;
}