#include "dfa.h"
#include "test_dfa.h"
#include "../nfa/nfa.h"
#include "../regex/regex.h"
#include "../nfa/thompson.h"

using namespace re;

int main()
{
    Re a = symbol("a");
    Re b = symbol("b");
    Re c = symbol("c");
    Re re = concatenate(alternate(a, b), closure(c));

    nfa::Nfa n = nfa::thompson(re);
    Dfa dfa = subset_construction(n);
    print_dfa(dfa);
}
