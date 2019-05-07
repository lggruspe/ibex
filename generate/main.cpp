#include "generate.h"
#include "../dfa/dfa.h"
#include "../regex/utilities.h"
#include "../regex/regex.h"
#include "../nfa/nfa.h"
#include "../thompson/thompson.h"
#include "../hopcroft/hopcroft.h"

int main()
{
  re_load_predefined();
  Re re = identifier;
  Nfa nfa = thompson(re);
  Dfa dfa = minimize(subset_construction(nfa));
  generate("scanner", dfa);
  extra("scanner");
}
