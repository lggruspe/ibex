#include "dfa.h"
#include "test_dfa.h"
#include "../nfa/nfa.h"
#include "../regex/regex.h"
#include "../nfa/thompson.h"

using namespace automata;

int main()
{
    regex::Re a = regex::symbol("a");
    regex::Re b = regex::symbol("b");
    regex::Re c = regex::symbol("c");
    regex::Re re = regex::concatenate(regex::alternate(a, b), 
            regex::closure(c));

    Nfa nfa = thompson(re);
    Dfa dfa = subset_construction(nfa);
    print(dfa);
}
