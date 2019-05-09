#include "hopcroft.h"
#include "../regex/regex.h"
#include "../regex/utilities.h"
#include "../nfa/thompson.h"
#include "dfa.h"
#include "test_dfa.h"

using namespace automata;

int main()
{
    regex::Re real = regex::real();
    Nfa nfa = thompson(real);
    Dfa dfa = minimize(subset_construction(nfa));
    print(dfa);
}
