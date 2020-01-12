#include "rnd/zsymbols.hpp"
#include <iostream>

using namespace rnd;

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
    for (const auto& [a, b]: r.to_set()) {
        std::cout << "[" << a << ", " << b << ")" << std::endl;
    }
    for (const auto& x: r.points) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
    const auto& cover = r.cover(ZRange(15, 15));
    for (const auto& [a, b]: cover) {
        std::cout << "[" << a << ", " << b << ")" << std::endl;
    }
}
