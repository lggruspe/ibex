#include "dfa.h"
#include "test_dfa.h"
#include "../nfa/nfa.h"
#include "../regex/regex.h"
#include "../thompson/thompson.h"

int main()
{
  Regex regex;
  Re a = regex.symbol('a');
  Re b = regex.symbol('b');
  Re c = regex.symbol('c');
  Re re = regex.concatenate(regex.alternate(a, b), regex.closure(c));
  Nfa nfa = thompson(re);
  Dfa dfa = subset_construction(nfa);

  print_dfa(dfa);
}
