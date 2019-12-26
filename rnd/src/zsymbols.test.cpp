#include "zsymbols.hpp"
#include <iostream>

int main()
{
    ZPartition p = {
        ZRange(1, 5),
        ZRange(2, 4),
        ZRange(1, 3),
        ZRange(10, 11),
        ZRange(1, 11),
        ZRange(17, 17),
    };

    ZPartition q = {
        ZRange(1, 5),
        ZRange(2, 4),
        ZRange(1, 3),
        ZRange(4, 9),
        ZRange(5, 18),
        ZRange(17, 1230),
    };

    auto r = p.combined(q);
    for (const auto& [a, b]: r) {
        std::cout << "[" << a << ", " << b << ")" << std::endl;
    }
    for (const auto& x: r.points) {
        std::cout << x << " ";
    }
    std::cout << std::endl;

    auto [lb, ub] = r.overlap_range(ZRange(15, 15));
    for (auto it = lb; it != ub; ++it) {
        const auto& [a, b] = *it;
        std::cout << "[" << a << ", " << b << ")" << std::endl;
    }
    std::cout << "lb = [" << (*lb).start << ", " << (*lb).end << ")" << std::endl;
    std::cout << "ub = [" << (*ub).start << ", " << (*ub).end << ")" << std::endl;
}
