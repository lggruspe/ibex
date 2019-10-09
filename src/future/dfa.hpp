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
    ClosedIntervalSet symbols;
    int start;  // only gets modified when minimized
    std::map<int, std::map<ClosedInterval, int>> transitions;
    std::set<int> accepts;

    Fsm(const Expr& expr);
};

std::ostream& operator<<(std::ostream& os, const Fsm& fsm)
{
    os << "start: " << fsm.start << std::endl;
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
    if (Q.find(1) != Q.end()) {
        fsm.accepts.insert(state);
    }
    while (!stack.empty()) {
        int qid = stack.back();
        stack.pop_back();
        fsm.transitions[qid];
        auto Q = states.value(qid);
        for (const auto& a: fsm.symbols) {
            auto R = epsilon_closure(closure, next_nfa_state(expr, Q, a));
            auto [rid, ok] = states.index(R);
            fsm.transitions[qid][a] = rid;
            if (ok) {
                // if index is newly inserted
                stack.push_back(rid);
                // add if accept state
                if (R.find(1) != R.end()) {
                    fsm.accepts.insert(rid);
                }
            }
        }
    }
}

struct Partition {
    std::map<int, std::set<int>> id_to_class;
    std::map<int, bool> id_is_accept;
    std::map<int, int> item_to_id;

    void insert(int id, int item, bool is_accept)
    {
        // assume item is not in any class yet
        id_to_class[id].insert(item);
        item_to_id[item] = id;
        id_is_accept[id] = is_accept;
    }

    void update(int new_id, int item)
    {
        int old_id = item_to_id[item];
        id_to_class[old_id].erase(item);
        bool is_accept = id_is_accept[old_id];
        insert(new_id, item, is_accept);
    }

    void bulk_insert(const std::set<int>& items, bool is_accept)
    {
        int id = id_to_class.size();
        id_is_accept[id] = is_accept;
        id_to_class[id] = items;
        for (const auto& q: items) {
            item_to_id[q] = id;
        }
    }
};

Partition first_partition(const Fsm& fsm)
{
    // 0: accepts, 1: nonaccepts
    Partition p;
    for (const auto& [q, _]: fsm.transitions) {
        int id = (fsm.accepts.find(q) == fsm.accepts.end());
        p.insert(id, q, !id);
    }
    return p;

    /* or:
    Partition p;
    std::set<int> nonaccepts;
    for (const auto& [q, _]: fsm.transitions) {
        if (auto it = fsm.accepts.find(q); it == fsm.accepts.end()) {
            nonaccepts.insert(q);
        }
    }
    if (!fsm.accepts.empty()) {
        p.bulk_insert(fsm.accepts, true);
    }
    if (!nonaccepts.empty()) {
        p.bulk_insert(nonaccepts, false);
    }
    return p;
    */
}

std::map<int, std::set<int>> map_transitions(
    const Partition& p,
    int id,
    const Fsm& fsm,
    const ClosedInterval& a)
{
    // outbound: id of destination -> source state
    std::map<int, std::set<int>> outbound;
    for (const auto& q: p.id_to_class.at(id)) {
        outbound[p.item_to_id.at(fsm.transitions.at(q).at(a))].insert(q);
    }
    return outbound;
}

void split(Partition& p, int id, const Fsm& fsm)
{
    for (const auto& a: fsm.symbols) {
        auto& cls = p.id_to_class.at(id);
        if (cls.size() <= 1) {
            break;
        }
        auto outbound = map_transitions(p, id, fsm, a);
        assert(!outbound.empty());
        for (auto it = std::next(outbound.begin()); it != outbound.end(); ++it) {
            const auto& Q = it->second;
            int new_id = p.id_to_class.size();
            for (const auto& q: Q) {
                p.update(new_id, q);
            }
        }
    }
}

void minimize(Fsm& fsm)
{
    auto p = first_partition(fsm);
    for (const auto& [id, _]: p.id_to_class) {
        split(p, id, fsm);
    }

    std::set<int> accepts;
    std::map<int, std::map<ClosedInterval, int>> transitions;
    for (const auto& [q, dq]: fsm.transitions) {
        for (const auto& a: fsm.symbols) {
            transitions[p.item_to_id[q]][a] = p.item_to_id[dq.at(a)];
        }
        if (p.id_is_accept[p.item_to_id[q]]) {
            accepts.insert(p.item_to_id[q]);
        }
    }
    fsm.transitions = transitions;
    fsm.accepts = accepts;
    fsm.start = p.item_to_id[0];
}

Fsm::Fsm(const Expr& expr)
{
    start = 0;
    subset_construction(*this, expr);
    minimize(*this);
}
