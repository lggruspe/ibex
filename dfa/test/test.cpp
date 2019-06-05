#include "nfa.h"
#include "hopcroft.h"
#include "regex2.h"
#include "utilities.h"
#include "dfa.h"
#include <iostream>

using namespace automata;
using namespace regex2;

int main()
{
    Expr re = identifier();
    Nfa nfa = thompson(re);
    Dfa dfa = subset_construction(nfa);
    std::cout << dfa << std::endl;

    dfa = minimize(dfa);
    std::cout << dfa << std::endl;
}
