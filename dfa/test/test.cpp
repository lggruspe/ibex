#include "nfa.h"
#include "hopcroft.h"
#include "regex2.h"
#include "utilities.h"
#include "dfa.h"
#include <iostream>

using namespace automata;
using namespace regex2;

void construct_and_print(Expr re)
{
    Nfa nfa = thompson(re);
    Dfa dfa = subset_construction(nfa);
    dfa = minimize(dfa);
    std::cout << dfa << std::endl;
}

void test_identifier()
{
    std::cout << "identifier" << std::endl;
    Expr re = identifier();
    construct_and_print(re);
}

void test_integer()
{
    std::cout << "integer" << std::endl;
    Expr re = integer();
    construct_and_print(re);
}

void test_number()
{
    std::cout << "number" << std::endl;
    Expr re = number();
    construct_and_print(re);
}

void test_empty()
{
    std::cout << "empty" << std::endl;
    Expr re = empty();
    construct_and_print(re);
}

void test_whitespace()
{
    std::cout << "whitespace" << std::endl;
    Expr re = whitespace();
    construct_and_print(re);
}

int main()
{
    test_number();
}
