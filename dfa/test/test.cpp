#include "nfa.h"
#include "hopcroft.h"
#include "regex.h"
#include "utilities.h"
#include "dfa.h"
#include "test.h"

using namespace automata;
using namespace regex;

int main()
{
    Expr re = identifier();
    Nfa nfa = thompson(re);
    Dfa dfa = minimize(subset_construction(nfa));
    print(dfa);
}
