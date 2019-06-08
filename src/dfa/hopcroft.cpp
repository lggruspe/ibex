#include "dfa/hopcroft.h"
#include "partition.h"
#include <iterator>
#include <list>
#include <map>
#include <set>

namespace automata 
{

int dfa_transition(const Dfa& dfa, int q, char a)
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

Dfa minimize(const Dfa& dfa)
{
    // get dfa states (keys of dfa.delta)
    std::vector<int> states;
    std::transform(dfa.delta.begin(), dfa.delta.end(),
            std::back_inserter(states),
            [](const std::pair<int, std::map<char, int>>& pair) {
                return pair.first;
            });

    // split partition between accept and nonaccept states
    Partition<int> partition(states.begin(), states.end());
    partition.split_by_set(dfa.accept);
    // split according to behavior on a (by pairwise comparisons)
    
    for (auto a: dfa.symbols) {
        partition.split_by_comparison([&dfa, &a, &partition](const int& p, const int& q) {
                    int pr = dfa_transition(dfa, p, a);
                    int qr = dfa_transition(dfa, q, a);
                    if (pr*qr < 0) {
                        return false;
                    }
                    if (pr < 0) {
                        return true;
                    }
                    return partition.index(pr) == partition.index(qr);
                });
    }

    // set start and accept states
    Dfa M;
    M.start = partition.index(dfa.start);
    for (auto f: dfa.accept) {
        M.accept.insert(partition.index(f));
    }

    // set transitions
    for (const auto& trans: dfa.delta) {
        int q = partition.index(trans.first);
        for (const auto& tran: trans.second) {
            const auto& a = tran.first;
            int r = partition.index(tran.second);
            M.add_transition(q, r, a);
        }
    }
    return M;
}

} // end namespace
