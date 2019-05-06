#include "../nfa/nfa.h"
#include <map>
#include <set>

struct Dfa {
  
};

std::map<int, std::set<int> > epsilon_closure(const Nfa& nfa)
{
  std::map<int, std::set<int> > closures;
  return closures;
}

Dfa subset_construction(const Nfa& nfa)
{
  Dfa dfa;
  return dfa;
}
