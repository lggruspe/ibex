#include "thompson.h"
#include "../regex/regex.h"
#include "../nfa/nfa.h"
#include "../regex/test_regex.h"
#include "../nfa/test_nfa.h"
#include <iostream>

int main()
{
  Regex regex;
  Re a = regex.symbol('a');
  Re b = regex.symbol('b');
  Re c = regex.symbol('c');
  Re re = regex.concatenate(regex.alternate(a, b), regex.closure(c));

  print_regex(re);
  std::cout << std::endl;

  Nfa nfa = thompson(re);

  print_nfa(nfa);
}
