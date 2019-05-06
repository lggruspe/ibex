#include "nfa.h"

int nfa_merge(Nfa& A, const Nfa& B)
{
  // add states of B into this (with renamed states)
  // returns offset to B states
  std::map<int, std::map<char, std::set<int> > > delta;
  int offset = A.add_state();
  for (auto it = B.delta.begin(); it != B.delta.end(); ++it) {
    int q = it->first;
    for (auto jt = it->second.begin(); jt != it->second.end(); ++jt) {
      char a = jt->first;
      for (auto r: jt->second) {
        delta[q+offset][a].insert(r+offset);
      }
    }
  }

  for (auto it = delta.begin(); it != delta.end(); ++it) {
    int q = it->first;
    for (auto jt = it->second.begin(); jt != it->second.end(); ++jt) {
      char a = jt->first;
      for (auto r: jt->second) {
        A.add_transition(q, a, r);
      }
    }
  }
  return offset;
}

Nfa nfa_symbol(char a)  // a = '\0' for epsilon
{
  Nfa nfa;
  int start = nfa.add_state(0);;
  int accept = nfa.add_state(1);;
  nfa.start = start;
  nfa.accept = accept;
  nfa.add_transition(start, a, accept);
  return nfa;
}

Nfa nfa_union(Nfa& A, const Nfa& B)
{
  // works even if A = B
  int offset = nfa_merge(A, B);
  int bs = B.start + offset;
  int bf = B.accept + offset;
  int start = A.add_state();
  int accept = A.add_state();

  A.add_transition(start, '\0', A.start);
  A.add_transition(start, '\0', bs);
  A.add_transition(A.accept, '\0', accept);
  A.add_transition(bf, '\0', accept);

  A.start = start;
  A.accept = accept;
  return A;
}

Nfa nfa_concatenation(Nfa& A, const Nfa& B)
{
  int offset = nfa_merge(A, B);
  int bs = B.start + offset;
  int bf = B.accept + offset;
  A.add_transition(A.accept, '\0', bs);
  A.accept = bf;
  return A;
}

Nfa nfa_closure(Nfa& A)
{
  int start = A.add_state();
  int accept = A.add_state();
  A.add_transition(start, '\0', A.start);
  A.add_transition(start, '\0', accept);
  A.add_transition(A.accept, '\0', A.start);
  A.add_transition(A.accept, '\0', accept);
  A.start = start;
  A.accept = accept;
  return A;
}
