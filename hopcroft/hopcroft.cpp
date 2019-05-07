#include "hopcroft.h"
#include <iterator>
#include <list>
#include <map>
#include <set>

int dfa_transition(const Dfa& dfa, int q, int a)
{
  // -1 means delta(q, a) dne
  auto it = dfa.delta.find(q);
  if (it != dfa.delta.end()) {
    auto jt = it->second.find(a);
    if (jt != it->second.end()) {
      return jt->second;
    }
  }
  return -1;
}

std::list<std::set<int> >::iterator split(const Dfa& dfa,
    std::list<std::set<int> >::iterator it, std::list<std::set<int> >& partition, char a)
{
  int q = *(it->begin());
  std::set<int> eq, neq;
  for (auto r: *it) {
    if (dfa_transition(dfa, q, a) == dfa_transition(dfa, r, a)) {
      eq.insert(r);
    } else {
      neq.insert(r);
    }
  }

  if (neq.empty()) {
    return std::next(it);
  }

  it = partition.erase(it);
  partition.push_back(eq);
  partition.push_back(neq);
  return it;
}

Dfa minimize(const Dfa& dfa)
{
  std::list<std::set<int> > partition;
  std::set<int> nonaccept;
  for (auto it = dfa.delta.begin(); it != dfa.delta.end(); ++it) {
    if (dfa.accept.find(it->first) == dfa.accept.end()) {
      nonaccept.insert(it->first);
    }
  }
  partition.push_back(dfa.accept);
  partition.push_back(nonaccept);

  for (auto a: dfa.symbols) {
    auto it = partition.begin();
    while (it != partition.end()) {
      it = split(dfa, it, partition, a);
    }
  }

  // construct M

  Dfa M;
  std::map<int, int> reps;    // state classes
  for (auto it = partition.begin(); it != partition.end(); ++it) {
    int rep = M.add_state();
    for (auto q: *it) {
      reps[q] = rep;
    }
  }

  M.start = reps[dfa.start];
  for (auto f: dfa.accept) {
    M.accept.insert(reps[f]);
  }

  for (auto it = dfa.delta.begin(); it != dfa.delta.end(); ++it) {
    int q = reps[it->first];
    for (auto jt = it->second.begin(); jt != it->second.end(); ++jt) {
      char a = jt->first;
      int r = reps[jt->second];
      M.add_transition(q, a, r);
    }
  }

  return M;
}
