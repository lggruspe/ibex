#include "hopcroft.h"
#include "../regex/regex.h"
#include "../regex/utilities.h"
#include "../thompson/thompson.h"
#include "../dfa/dfa.h"
#include "../dfa/test_dfa.h"

int main()
{
  re_load_predefined();
  Re re = real;
  Nfa nfa = thompson(re);
  Dfa dfa = minimize(subset_construction(nfa));
  print_dfa(dfa);
}
