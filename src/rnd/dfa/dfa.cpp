#include "dfa.h"
#include "enumeration2.h"
#include "partition.h"
#include "rnd/alphabet/alphabet.hpp"
#include "rnd/nfa/nfa.h"
#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <set>

namespace rnd
{
namespace dfa 
{

Dfa subset_construction(const nfa::Nfa&);
Dfa minimize(const Dfa&);

Dfa::Dfa(const nfa::Nfa& nfa)
{
    *this = minimize(subset_construction(nfa));
}

Dfa::Dfa(regex::Expr expr)
{
    *this = minimize(subset_construction(nfa::Nfa(expr)));
}

int add_state(Dfa& dfa, int q)
{
    dfa.delta[q];
    return q;
}

int add_state(Dfa& dfa)
{
    int q = dfa.delta.empty() ? 0 : dfa.delta.rbegin()->first + 1;
    return add_state(dfa, q);
}

void add_transition(Dfa& dfa, int q, int r, SymbolRange a)
{
    add_state(dfa, q);
    add_state(dfa, r);
    dfa.delta[q][a] = r;
    if (!is_empty(a)) {
        dfa.symbols.insert(a);
    }
}

void compute_predecessors(const nfa::Nfa& nfa, 
        std::map<int, std::set<int> >& predecessors)
{
    // assume predecessors is empty
    auto epsilon = symbol_range(0, 0);
    for (auto it = nfa.delta.begin(); it != nfa.delta.end(); ++it) {
        int p = it->first;
        if (it->second.count(epsilon) > 0) {     // TODO don't search entire it->second
            for (auto q: it->second.at(epsilon)) {
                predecessors[q].insert(p);
            }
        }
    }
}

std::map<int, std::set<int>> epsilon_closure(const nfa::Nfa& nfa)
{
    std::map<int, std::set<int> > closures;
    std::map<int, std::set<int> > predecessors;
    compute_predecessors(nfa, predecessors);

    std::list<int> queue;
    for (auto it = nfa.delta.begin(); it != nfa.delta.end(); ++it) {
        int q = it->first;
        closures[q].insert(q);
        queue.push_back(q);
    }

    auto epsilon = symbol_range(0, 0);
    while (!queue.empty()) {
        int q = queue.front();
        queue.pop_front();

        std::set<int> closure;
        closure.insert(q);

        if (nfa.delta.at(q).count(epsilon) > 0) {        // TODO Only count one
            for (auto r: nfa.delta.at(q).at(epsilon)) {
                // closure = closure.union(closures[r])
                std::copy(closures[r].begin(), closures[r].end(),
                        std::inserter(closure, closure.begin()));
            }
        }

        if (closure != closures[q]) {
            closures[q] = closure;
            for (auto p: predecessors[q]) {
                queue.push_back(p);
            }
        }
    }
    return closures;
}

std::set<int> next_state(
        const nfa::Nfa& nfa, 
        const std::set<int>& Q, 
        SymbolRange a,
        const std::map<int, std::set<int>>& closures)
{
    // TODO pass R by ref?
    std::set<int> R;
    for (auto q: Q) {
        std::set<int> Rq;
        if (nfa.delta.at(q).count(a) > 0) {
            for (auto r: nfa.delta.at(q).at(a)) {
                Rq.insert(r);
            }
        }

        // R = R.union(Rq)
        std::copy(Rq.begin(), Rq.end(), std::inserter(R, R.begin()));
    }

    std::set<int> S;
    for (auto r: R) {
        // S = S.union(closures[r])
        std::copy(closures.at(r).begin(), closures.at(r).end(),
                std::inserter(S, S.begin()));
    }
    return S;
}

Dfa subset_construction(const nfa::Nfa& nfa)
{
    std::map<int, std::set<int> > closures = epsilon_closure(nfa);
    Enumeration<std::set<int>> names;
    
    Dfa dfa;
    dfa.start = add_state(dfa, names.insert(closures[nfa.start]));

    std::list<int> queue;
    queue.push_back(dfa.start);

    // check if dfa.start is an accept state
    {
        auto closure = closures[nfa.start];
        if (closure.find(nfa.accept) != closure.end()) {
            dfa.accept.insert(dfa.start);
        }
    }

    /// subset construction
    while (!queue.empty()) {
        int Q = queue.front();
        queue.pop_front();
        for (const auto& a: nfa.symbols) {
            std::set<int> R = next_state(nfa, names.value(Q), a, closures);
            if (R.empty()) {
                continue;
            }

            if (!names.has_value(R)) {
                int name = names.insert(R);
                add_state(dfa, name);
                queue.push_back(name);
                for (auto r: R) {
                    if (r == nfa.accept) {
                        dfa.accept.insert(name);
                    }
                }
            }
            add_transition(dfa, Q, names.index(R), a);
        }
    }
    return dfa;
}

int dfa_transition(const Dfa& dfa, int q, SymbolRange a)
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
            [](const std::pair<int, std::map<SymbolRange, int>>& pair) {
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
    add_state(M, M.start);
    for (auto f: dfa.accept) {
        auto id = partition.index(f);
        M.accept.insert(id);
        add_state(M, id);
    }

    // set transitions
    for (const auto& trans: dfa.delta) {
        int q = partition.index(trans.first);
        add_state(M, q);
        for (const auto& tran: trans.second) {
            const auto& a = tran.first;
            int r = partition.index(tran.second);
            add_transition(M, q, r, a);
        }
    }
    return M;
}

std::ostream& operator<<(std::ostream& out, const Dfa& dfa)
{
    out << "start: " << dfa.start << std::endl;
    out << "accept: ";
    for (auto f: dfa.accept) {
        out << f << " ";
    }
    out << std::endl;
    out << "delta: " << std::endl;
    for (auto it = dfa.delta.begin(); it != dfa.delta.end(); ++it) {
        int q = it->first;
        for (auto jt = it->second.begin(); jt != it->second.end(); ++jt) {
            SymbolRange a = jt->first;
            int r = jt->second;
            out << "d(" << q << ", ";
            out << "[" << (char)(a.start) << ", " << (char)(a.end) << "]";
            out << ") = " << r << std::endl;
        }
    }
    out << std::endl;
    return out;
}

} // end namespace
}
