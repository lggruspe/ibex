#include "cis.hpp"

int main()
{
    ClosedIntervalSet s = {
        ClosedInterval(1, 3),
        ClosedInterval(4, 5),
        ClosedInterval(0, 100),
    };

    for (const auto& a: s) {
        std::cout << a << std::endl;
    }
    std::cout << std::endl;

    ClosedIntervalSet t = {
        ClosedInterval(2, 4),
        ClosedInterval(5, 9),
    };

    for (const auto& a: t) {
        std::cout << a << std::endl;
    }
    std::cout << std::endl;

    auto u = combine_interval_sets(s, t);

    for (const auto& a: u) {
        std::cout << a << std::endl;
    }
    std::cout << std::endl;
}
