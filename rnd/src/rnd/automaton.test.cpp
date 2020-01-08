#include "rnd/automaton.hpp"
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

void test_closure()
{
    std::cout << "closure" << std::endl;
    NExpr expr(ZRange('a', 'a'+1));
    expr = closure(expr);
    std::cout << expr << std::endl;
    Automaton fsm(expr);
    std::cout << fsm << std::endl;
}

void test_alternate()
{
    std::cout << "alternate" << std::endl;
    NExpr expr(ZRange('a', 'a'+1));
    NExpr other(ZRange('b', 'b'+1));
    expr = alternate(expr, other);
    std::cout << expr << std::endl;
    Automaton fsm(expr);
    std::cout << fsm << std::endl;
}

void test_concatenate()
{
    std::cout << "concatenate" << std::endl;
    NExpr expr(ZRange('a', 'a'+1));
    NExpr other(ZRange('b', 'b'+1));
    expr = concatenate(expr, other);
    std::cout << expr << std::endl;
    Automaton fsm(expr);
    std::cout << fsm << std::endl;
}

int main()
{
    test_closure();
    test_alternate();
    test_concatenate();
}
