#include "test_regex.h"
#include <iostream>

void print_regex(Re re)
{
  auto sp = re.lock();
  if (!sp) {
    std::cout << "nil";
  } else if (sp->value == '*') {
    std::cout << "(*, ";
    print_regex(sp->lhs);
    std::cout << ")";
  } else if (sp->value == '+') {
    std::cout << "(+, ";
    print_regex(sp->lhs);
    std::cout << ", ";
    print_regex(sp->rhs);
    std::cout << ")";
  } else if (sp->value == '|') {
    std::cout << "(|, ";
    print_regex(sp->lhs);
    std::cout << ", ";
    print_regex(sp->rhs);
    std::cout << ")";
  } else {
    std::cout << sp->value;
  }
  sp.reset();
}
