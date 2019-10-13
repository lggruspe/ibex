#pragma once
#include "cis.hpp"
#include "nfa.hpp"
#include <iostream>
#include <map>

#include "handles.hpp"
#include <cassert>
#include <vector>

#include <algorithm>
#include <iterator>
#include <list>
#include <utility>

struct Fsm {
    // invariant: start state is always 0
    ClosedIntervalSet symbols;
    std::map<int, std::map<ClosedInterval, int>> transitions;
    std::set<int> accepts;

    Fsm(const Expr& expr);
};

std::ostream& operator<<(std::ostream& os, const Fsm& fsm)
{
    os << "accept: ";
    for (const auto& q: fsm.accepts) {
        os << q << " ";
    }
    os << std::endl;
    for (const auto& [q, dq]: fsm.transitions) {
        assert(dq.size() > 0);
        for (const auto& [a, r]: dq) {
            os << "d(" << q << ", " << a << ") = " << r << std::endl;
        }
    }
    return os;
}

std::set<int> next_nfa_state(
    const Expr& expr, 
    const std::set<int>& Q,
    const ClosedInterval& a)
{
    std::set<int> res;
    for (const auto& q: Q) {
        auto it = expr.transitions.at(q).find(a);
        if (it != expr.transitions.at(q).end()) {
            const auto& R = it->second;
            res.insert(R.begin(), R.end());
        }
    }
    return res;
}

std::map<int, std::set<int>> epsilon_predecessors(const Expr& expr)
{
    ClosedInterval eps;
    std::map<int, std::set<int>> pred;
    for (auto& [q, dq]: expr.transitions) {
        if (auto it = dq.find(eps); it != dq.end()) {
            for (const auto& r: it->second) {
                pred[r].insert(q);
            }
        }
    }
    return pred;
}

std::map<int, std::set<int>> epsilon_closures(const Expr& expr)
{
    ClosedInterval eps;
    std::map<int, std::set<int>> closure;
    std::vector<int> stack;
    for (const auto& [q, dq]: expr.transitions) {
        stack.push_back(q);
        closure[q].insert(q);
        if (auto it = dq.find(eps); it != dq.end()) {
            const auto& R = it->second;
            closure[q].insert(R.begin(), R.end());
        }
    }
    auto predecessor = epsilon_predecessors(expr);
    while (!stack.empty()) {
        int q = stack.back();
        stack.pop_back();
        for (const auto& p: predecessor[q]) {
            int before = closure[p].size();
            closure[p].insert(closure[q].begin(), closure[q].end());
            int after = closure[p].size();
            if (before != after) {
                // TODO don't insert if already in stack
                stack.push_back(p);
            }
        }
    }
    return closure;
}

std::set<int> epsilon_closure(
    const std::map<int, std::set<int>>& closure,
    const std::set<int>& Q)
{
    std::set<int> R;
    for (const auto& q: Q) {
        R.insert(closure.at(q).begin(), closure.at(q).end());
    }
    return R;
}

void subset_construction(Fsm& fsm, const Expr& expr)
{
    auto closure = epsilon_closures(expr);
    HandleSet<std::set<int>> states;
    fsm.symbols = expr.symbols;
    auto Q = closure[0];
    auto [state, _] = states.index(Q);
    std::vector<int> stack = {state};
    while (!stack.empty()) {
        int qid = stack.back();
        stack.pop_back();
        fsm.transitions[qid];
        auto Q = states.value(qid);
        if (Q.find(1) != Q.end()) {
            fsm.accepts.insert(qid);
        }
        for (const auto& a: fsm.symbols) {
            auto R = epsilon_closure(closure, next_nfa_state(expr, Q, a));
            auto [rid, ok] = states.index(R);
            fsm.transitions[qid][a] = rid;
            if (ok) {
                // if index is newly inserted
                stack.push_back(rid);
            }
        }
    }
}

class Partition {
    std::map<int, std::set<int>> id_to_class;
    std::map<int, int> item_to_id;

public:
    auto begin()
    {
        return id_to_class.begin();
    }

    auto end()
    {
        return id_to_class.end();
    }

    int size() const
    {
        return id_to_class.size();
    }

    void set(int id, int item)
    {
        auto it = item_to_id.find(item);
        if (it != item_to_id.end()) {
            id_to_class[it->second].erase(item);
        }
        item_to_id[item] = id;
        id_to_class[id].insert(item);
    }

    int classify(int item) const
    {
        return item_to_id.at(item);
    }

    int to_state(int q) const
    {
        if (q == 0) {
            return 0;
        }
        int id = classify(q);
        if (id == 0) {
            return classify(0);
        }
        return id;
    }
};

Partition first_partition(const Fsm& fsm)
{
    // 0: accepts, 1: nonaccepts
    Partition p;
    for (const auto& [q, _]: fsm.transitions) {
        int id = fsm.accepts.find(q) == fsm.accepts.end();
        p.set(id, q);
    }
    return p;
}

std::map<int, std::set<int>> transition_groups(
    const Partition& p,
    const std::set<int>& cls,
    const Fsm& fsm,
    const ClosedInterval& a)
{
    // groups states in the same class by their outbound transitions at a
    // group keys don't actually mean anything outside the function
    std::map<int, std::set<int>> groups;
    for (const auto& q: cls) {
        const auto& dq = fsm.transitions.at(q);
        int r = dq.at(a);
        groups[p.classify(r)].insert(q);
    }
    return groups;
}

bool split_by_transition(
    Partition& p,
    const std::set<int>& cls,
    const Fsm& fsm,
    const ClosedInterval& a)
{
    bool changed = false;
    auto groups = transition_groups(p, cls, fsm, a);
    for (auto it = std::next(groups.begin()); it != groups.end(); ++it) {
        const auto& Q = it->second;
        changed = true;
        int id = p.size();
        for (const auto& q: Q) {
            p.set(id, q);
        }
    }
    return changed;
}

bool split(Partition& p, const std::set<int>& cls, const Fsm& fsm)
{
    bool changed = false;
    for (const auto& a: fsm.symbols) {
        if (cls.size() <= 1) {
            break;
        }
        changed = changed || split_by_transition(p, cls, fsm, a);
    }
    return changed;
}

Partition refined_states(const Fsm& fsm)
{
     //  TODO test if the resulting state space is a permutation of 0, 1, ..., k
    //  for some k
    auto p = first_partition(fsm);
    for (;;) {
        bool changed = false;
        for (const auto& [_, cls]: p) {
            changed = changed || split(p, cls, fsm);
        }
        if (!changed) {
            break;
        }
    }
    return p;
}

void minimize(Fsm& fsm)
{
    auto p = refined_states(fsm);
    std::map<int, std::map<ClosedInterval, int>> transitions;
    for (const auto& [q, dq]: fsm.transitions) {
        auto& Tq = transitions[p.to_state(q)];
        for (const auto& a: fsm.symbols) {
            Tq[a] = p.to_state(dq.at(a));
        }
    }
    std::set<int> accepts;
    for (const auto& q: fsm.accepts) {
        accepts.insert(p.to_state(q));
    }
    fsm.transitions = transitions;
    fsm.accepts = accepts;
}

Fsm::Fsm(const Expr& expr)
{
    subset_construction(*this, expr);
    minimize(*this);
}
