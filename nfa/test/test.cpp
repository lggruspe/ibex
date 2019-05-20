#include "nfa.h"
#include "regex.h"
#include "utilities.h"
#include "test.h"
#include <iostream>

using namespace regex;
using namespace automata;

int main()
{
    Expr intr = integer();
    Expr re = real();
    Expr id = identifier();

    std::cout << "Integer" << std::endl;
    Nfa nfa = thompson(intr);
    print(nfa);

    std::cout << "Real" << std::endl;
    nfa = thompson(re);
    print(nfa);

    std::cout << "Identifier" << std::endl;
    nfa = thompson(id);
    print(nfa);
}
