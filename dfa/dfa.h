#ifndef DFA_H
#define DFA_H

#include "../nfa/nfa.h"
#include <map>
#include <set>

struct Dfa {
  int start;
  std::set<int> accept;
  std::map<int, std::map<char, int> > delta;
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

  void add_transition(int q, int a, int r)
  {
    this->add_state(q);
    this->add_state(r);
    this->delta[q][a] = r;
    this->symbols.insert(a);  // check if a != '\0'?
  }
};

Dfa subset_construction(const Nfa&);

#endif
