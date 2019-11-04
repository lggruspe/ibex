#include "zpartition.h"
#include <iostream>

using namespace poc;

std::ostream& operator<<(std::ostream& os, const ZPartition& p)
{
    os << "{ ";
    for (const auto& [a, b]: p) {
        os << "[" << a << ", " << b << ") ";
    }
    return os << "}";
}

int main()
{
    ZPartition p;
    p.insert(2, 3);
    p.insert(1, 4);
    std::cout << p << std::endl;
    
    ZPartition q;
    q.insert(2, 4);
    q.insert(9, 11);
    std::cout << q << std::endl;

    auto r = p.combined(q);
    std::cout << r << std::endl;

    auto [lb, ub] = r.overlap_range(3, 10);
    for (auto it = lb; it != ub; ++it) {
        auto [a, b] = *it;
        std::cout << "[" << a << ", " << b << ")" << std::endl;
    }
}
