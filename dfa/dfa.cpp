#include "dfa.h"
#include "enumeration.h"
#include <algorithm>
#include <list>

namespace automata 
{
void compute_predecessors(const Nfa& nfa, 
        std::map<int, std::set<int> >& predecessors)
{
    // assume predecessors is empty
    for (auto it = nfa.delta.begin(); it != nfa.delta.end(); ++it) {
        int p = it->first;
        if (it->second.count("") > 0) {     // TODO don't search entire it->second
            for (auto q: it->second.at("")) {
                predecessors[q].insert(p);
            }
        }
    }
}

std::map<int, std::set<int> > epsilon_closure(const Nfa& nfa)
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

    while (!queue.empty()) {
        int q = queue.front();
        queue.pop_front();

        std::set<int> closure;
        closure.insert(q);

        if (nfa.delta.at(q).count("") > 0) {        // TODO Only count one
            for (auto r: nfa.delta.at(q).at("")) {
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

std::set<int> next_state(const Nfa& nfa, const std::set<int>& Q, const std::string& a,
        const std::map<int, std::set<int> >& closures)
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

Dfa subset_construction(const Nfa& nfa)
{
    std::map<int, std::set<int> > closures = epsilon_closure(nfa);
    Enumeration<std::set<int>> names;
    
    Dfa dfa;
    dfa.start = dfa.add_state(names.insert(closures[nfa.start]));

    std::list<int> queue;
    queue.push_back(dfa.start);

    while (!queue.empty()) {
        int Q = queue.front();
        queue.pop_front();
        for (const std::string& a: nfa.symbols) {
            std::set<int> R = next_state(nfa, names.value(Q), a, closures);
            if (R.empty()) {
                continue;
            }

            if (!names.has_value(R)) {
                int name = names.insert(R);
                dfa.add_state(name);
                queue.push_back(name);
                for (auto r: R) {
                    if (r == nfa.accept) {
                        dfa.accept.insert(name);
                    }
                }
            }
            dfa.add_transition(Q, a, names.index(R));
        }
    }
    return dfa;
}

} // end namespace
