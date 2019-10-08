#pragma once
#include "cis.hpp"
#include <iostream>
#include <map>
#include <set>

struct Expr {
    // invariant: start = 0, accept = 1
    // symbols shouldn't contain "empty" symbols
    ClosedIntervalSet symbols;
    std::map<int, std::map<ClosedInterval, std::set<int>>> transitions;

    Expr()
    {
        transitions[0];
        transitions[1];
    }

    Expr(const ClosedInterval& a)
    {
        transitions[0][a].insert(1);
        transitions[1];
        if (a) {
            insert_interval(symbols, a);
        }
    }
};

Expr closure(const Expr& expr)
{
    Expr res;
    res.symbols = expr.symbols;
    for (const auto& [q, dq]: expr.transitions) {
        for (const auto& [a, R]: dq) {
            for (const auto& r: R) {
                res.transitions[q+2][a].insert(r+2);
            }
        }
    }
    ClosedInterval eps;
    res.transitions[0][eps].insert(1);
    res.transitions[0][eps].insert(2);
    res.transitions[3][eps].insert(1);
    res.transitions[3][eps].insert(2);
    return res;
}

void copy_transitions(
    Expr& A,
    const Expr& B,
    const ClosedIntervalSet& symbols, 
    int offset)
{
    // B[0] -> A[offset]
    // assume symbols is the combined alphabet of A and B
    A.transitions[offset];
    A.transitions[offset+1];
    for (const auto& [q, dq]: B.transitions) {
        for (const auto& [a, R]: dq) {
            auto [lb, ub] = overlap_range(symbols, a);  // TODO factor out and use unordered_map?
            for (const auto& r: R) {
                for (auto it = lb; it != ub; it++) {
                    A.transitions[q+offset][*it].insert(r+offset);
                }
            }
        }
    }
}

Expr alternate(const Expr& A, const Expr& B)
{
    Expr res;
    res.symbols = combine_interval_sets(A.symbols, B.symbols);
    copy_transitions(res, A, res.symbols, 2);
    copy_transitions(res, B, res.symbols, A.transitions.size() + 2);

    int offset = A.transitions.size()+2;
    ClosedInterval eps;
    res.transitions[0][eps].insert(2);
    res.transitions[0][eps].insert(offset);
    res.transitions[3][eps].insert(1);
    res.transitions[offset+1][eps].insert(1);
    return res;
}

Expr concatenate(const Expr& A, const Expr& B)
{
    Expr res;
    res.symbols = combine_interval_sets(A.symbols, B.symbols);
    copy_transitions(res, A, res.symbols, 2);
    copy_transitions(res, B, res.symbols, A.transitions.size() + 2);

    int offset = A.transitions.size() + 2;
    ClosedInterval eps;
    res.transitions[0][eps].insert(2);
    res.transitions[3][eps].insert(offset);
    res.transitions[offset+1][eps].insert(1);
    return res;
}

std::ostream& operator<<(std::ostream& os, const Expr& expr)
{
    for (const auto& [q, dq]: expr.transitions) {
        for (const auto& [a, R]: dq) {
            for (const auto& r: R) {
                os << "d(" << q << ", " << a << ") = " << r << std::endl;
            }
        }
        if (dq.empty()) {
            os << "d(" << q << ", .) = ." << std::endl;
        }
    }
    return os;
}
