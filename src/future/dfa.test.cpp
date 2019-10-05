#include "dfa.h"
#include "nfa2.hpp"
#include "cis.hpp"
#include <iostream>

int main()
{
    Expr a(ClosedInterval(65, 65));
    //Expr expr = alternate(a, b);
    Expr expr = closure(a);
    std::cout << expr << std::endl;
    Fsm fsm(expr);
    std::cout << fsm << std::endl;
}
