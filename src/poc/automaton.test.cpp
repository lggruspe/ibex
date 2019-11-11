#include "automaton.hpp"
#include "nexpr.hpp"
#include "zsymbols.hpp"
#include <iostream>

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

std::ostream& operator<<(std::ostream& os, const Automaton& fsm)
{
    for (const auto& [q, dq]: fsm.states) {
        for (const auto& [a, r]: dq) {
            os << "d(" << q << ", [" << a.start << ", " << a.end << ")) = " << r << std::endl;
        }
    }
    os << "accepts: ";
    for (const auto& f: fsm.accepts) {
        os << f << " ";
    }
    os << "\nerror: " << fsm.error;
    return os;
}

int main()
{
    NExpr expr(ZRange('a', 'a'+1));
    std::cout << expr << std::endl;
    Automaton fsm(expr);
    std::cout << fsm << std::endl;
}
