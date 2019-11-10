#include "automaton.hpp"
#include "nexpr.hpp"
#include "zsymbols.hpp"
#include <iostream>

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
    return os;
}

int main()
{
    NExpr expr(ZRange('a', 'a'+1));
    Automaton fsm(expr);
    std::cout << fsm << std::endl;
}
