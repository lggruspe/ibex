#include "hopcroft.h"
#include "../regex/regex.h"
#include "../thompson/thompson.h"
#include "../dfa/dfa.h"
#include "../dfa/test_dfa.h"

int main()
{
  Regex regex;
  Re a = regex.symbol('a');
  Re b = regex.symbol('b');
  Re c = regex.symbol('c');
  Re re = regex.concatenate(regex.alternate(a, b), regex.closure(c));
  Nfa nfa = thompson(re);
  Dfa dfa = subset_construction(nfa);
  Dfa M = minimize(dfa);

  print_dfa(M);
}
