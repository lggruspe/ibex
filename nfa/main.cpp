#include "nfa.h"
#include "test_nfa.h"

int main()
{
    nfa::Nfa a = nfa::symbol("a");
    nfa::print(a);
    nfa::Nfa b = nfa::symbol("b");
    nfa::print(b);

    nfa::Nfa c = nfa::alternate(a, a);
    nfa::print(c);
}
