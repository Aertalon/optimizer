#include "src/stdx/traits.hpp"

#include "boost/ut.hpp"

// NOLINTBEGIN(readability-magic-numbers)

auto main() -> int
{
    using namespace boost::ut;

    test("bounded integer sequence") = [] {
        using A = opt::stdx::bounded_integer_sequence<std::size_t, 5U, 10U>;
        using B = std::integer_sequence<std::size_t, 5U, 6U, 7U, 8U, 9U>;

        expect(constant<std::is_same<A, B>::value>);

        using C = opt::stdx::offset_sequence<
            std::size_t,
            5U,
            std::integer_sequence<std::size_t, 1U, 2U>>;
        using D = std::integer_sequence<std::size_t, 6U, 7U>;

        expect(constant<std::is_same<C::type, D>::value>);
    };
}

// NOLINTEND(readability-magic-numbers)
