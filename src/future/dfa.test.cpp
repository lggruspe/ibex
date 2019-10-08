#include "dfa.hpp"
#include "nfa.hpp"
#include "cis.hpp"
#include <iostream>

void test_union()
{
    std::cout << "union" << std::endl;
    Expr a(ClosedInterval(65, 65));
    Expr b(ClosedInterval(66, 66));
    Expr expr = alternate(a, b);
    std::cout << expr << std::endl;
    Fsm fsm(expr);
    std::cout << fsm << std::endl;
}

void test_concatenation()
{
    std::cout << "concatenation" << std::endl;
    Expr a(ClosedInterval(65, 65));
    Expr b(ClosedInterval(66, 66));
    Expr expr = concatenate(a, b);
    std::cout << expr << std::endl;
    Fsm fsm(expr);
    std::cout << fsm << std::endl;
}

void test_closure()
{
    std::cout << "closure" << std::endl;
    Expr a(ClosedInterval(65, 65));
    Expr expr = closure(a);
    std::cout << expr << std::endl;
    Fsm fsm(expr);
    std::cout << fsm << std::endl;
}

int main()
{
    test_union();
    test_concatenation();
    test_closure();
}
