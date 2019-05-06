#ifndef NFA_H
#define NFA_H

#include <map>
#include <set>

struct Nfa {
  std::map<int, std::map<char, std::set<int> > > delta;
  int start, accept;
  std::set<char> symbols;

  int add_state(int q)
  {
    this->delta[q];
    return q;
  }

  int add_state()
  {
    int q = this->delta.empty() ? 0 : this->delta.rbegin()->first + 1;
    return this->add_state(q);
  }

  void add_transition(int q, char a, int r)
  {
    this->add_state(q);
    this->add_state(r);
    this->delta[q][a].insert(r);
    this->symbols.insert(a);
  }
};

Nfa nfa_symbol(char);
Nfa nfa_union(Nfa&, const Nfa&);
Nfa nfa_concatenation(Nfa&, const Nfa&);
Nfa nfa_closure(Nfa&);

#endif
