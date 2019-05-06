#include "nfa.h"
#include "test_nfa.h"

int main()
{
  Nfa a = nfa_symbol('a');
  print_nfa(a);
  Nfa b = nfa_symbol('b');
  print_nfa(b);

  Nfa c = nfa_union(a, a);
  print_nfa(c);
}
