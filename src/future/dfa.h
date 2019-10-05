#pragma once
#include "cis.hpp"
#include "nfa2.hpp"
#include <iostream>
#include <map>

#include "uhs.hpp"
#include <cassert>
#include <vector>

struct Fsm {
    ClosedIntervalSet symbols;
    std::map<int, std::map<ClosedInterval, int>> transitions;
    std::set<int> accepts;

    //Fsm() {}
    Fsm(const Expr& expr);

};

std::ostream& operator<<(std::ostream& os, const Fsm& fsm)
{
    os << "start: " << 0 << std::endl;
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
        auto it = dq.find(eps);
        if (it != dq.end()) {
            for (const auto& r: it->second) {
                pred[q].insert(r);
            }
        }
    }
    return pred;
}

std::map<int, std::set<int>> epsilon_closures(const Expr& expr)
{
    ClosedInterval eps;
    auto predecessor = epsilon_predecessors(expr);
    std::map<int, std::set<int>> closure;
    std::vector<int> stack;
    for (const auto& [q, dq]: expr.transitions) {
        stack.push_back(q);
        closure[q] = {q};
        auto it = dq.find(eps);
        if (it != dq.end()) {
            const auto& R = it->second;
            closure[q].insert(R.begin(), R.end());
        }
    }
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
    UniqueHandleSet<std::set<int>> states;
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
            if (R.find(1) != R.end()) {
                fsm.accepts.insert(rid);
            }
            if (ok) {
                // if index is newly inserted
                stack.push_back(rid);
            }
        }
    }
}

void minimize(Fsm& fsm)
{
    fsm.transitions[0];
    // TODO
}

Fsm::Fsm(const Expr& expr)
{
    subset_construction(*this, expr);
    minimize(*this);
}
