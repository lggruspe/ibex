#pragma once
#include "../nfa/nfa.h"
#include "../regex/regex.h"

Nfa thompson(std::weak_ptr<RegexTree> expr)
{
  auto sp = expr.lock();
  if (sp->leaf) {
    return nfa_symbol(sp->value);
  } else if (sp->value == '+') {
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
  }
  return Nfa();
}
