#include "thompson.h"

Nfa thompson(Re re)
{
  auto sp = re.lock();
  if (sp->value == '+') {
    Nfa A = thompson(sp->lhs);
    Nfa B = thompson(sp->rhs);
    return nfa_concatenation(A, B);
  } else if (sp->value == '|') {
    Nfa A = thompson(sp->lhs);
    Nfa B = thompson(sp->rhs);
    return nfa_union(A, B);
  } else if (sp->value == '*') {
    Nfa A = thompson(sp->lhs);
    return nfa_closure(A);
  } else {
    return nfa_symbol(sp->value);
  }
}
