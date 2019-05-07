#include "hopcroft.h"
#include <iterator>
#include <map>
#include <set>

int dfa_transition(const Dfa& dfa, int q, int a)
{
  // -1 means the delta(q, a) dne
  auto it = dfa.delta.find(q);
  if (it != dfa.delta.end()) {
    auto jt = it->second.find(a);
    if (jt != it->second.end()) {
      return jt->second;
    }
  }
  return -1;
}

// TODO should cls be an iterator instead?
bool split_by_symbol(const Dfa& dfa, const std::set<int>& cls,
    std::set<std::set<int> >& partition, char a)
{
  // return true if modified
  // assume cls is nonempty
  int q = *(cls.begin());
  std::set<int> eq, neq;
  eq.insert(q);
  for (auto it = std::next(cls.begin()); it != cls.end(); ++it) {
    if (dfa_transition(dfa, q, a) == dfa_transition(dfa, *it, a)) {
      eq.insert(*it);
    } else {
      neq.insert(*it);
    }
  } 

  if (neq.empty()) {
    return false;
  }

  partition.erase(cls);
  partition.insert(eq);
  partition.insert(neq);
  return false;
}

bool split(const Dfa& dfa, const std::set<int>& cls, 
    std::set<std::set<int> >& partition)
{
  for (char a: dfa.symbols) {
    if (split_by_symbol(dfa, cls, partition, a)) {
      return true;
    }
  }
  return false;
}

Dfa minimize(const Dfa& dfa)
{
  std::set<std::set<int> > partition;
  std::set<int> nonaccept;
  for (auto it = dfa.delta.begin(); it != dfa.delta.end(); ++it) {
    if (dfa.accept.find(it->first) == dfa.accept.end()) {
      nonaccept.insert(it->first);
    }
  }
  partition.insert(dfa.accept);
  partition.insert(nonaccept);

  bool changed = true;
  while (changed) {
    changed = false;
    for (auto cls: partition) {
      if (changed = split(dfa, cls, partition)) {
        break;
      }
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
