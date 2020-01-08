#include "rnd/nexpr.hpp"
#include "rnd/zsymbols.hpp"
#include <iostream>

using namespace rnd;

std::ostream& operator<<(std::ostream& os, const NExpr& expr)
{
    for (const auto& [q, dq]: expr.states) {
        os << "q = " << q << std::endl;
        for (const auto& [a, R]: dq) {
            os << "[" << a.start << ", " << a.end << "): ";
            for (const auto& r: R) {
                os << r << " ";
            }
            os << std::endl;
        }
    }
    return os;
}

int main()
{
    NExpr a(ZRange('a', 'a'+1));
    NExpr b(ZRange('b', 'b'+1));
    std::cout << alternate(a, b) << std::endl;
}
