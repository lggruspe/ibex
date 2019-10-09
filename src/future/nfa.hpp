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

bool is_empty_expr(const Expr& expr)
{
    return expr.symbols.empty() && expr.transitions.at(0).empty() &&
        expr.transitions.at(1).empty();
}

Expr closure(const Expr& expr)
{
    ClosedInterval eps;
    if (is_empty_expr(expr)) {
        return Expr(eps);
    }
    Expr res;
    res.symbols = expr.symbols;
    for (const auto& [q, dq]: expr.transitions) {
        for (const auto& [a, R]: dq) {
            for (const auto& r: R) {
                res.transitions[q+2][a].insert(r+2);
            }
        }
        // also copy epsilon-transitions if any
        if (auto it = dq.find(eps); it != dq.end()) {
            for (const auto& r: it->second) {
                res.transitions[q+2][eps].insert(r+2);
            }
        }
    }
    res.transitions[0][eps].insert(1);
    res.transitions[0][eps].insert(2);
    res.transitions[3][eps].insert(1);
    res.transitions[3][eps].insert(2);
    return res;
}

void copy_transitions(
    Expr& A,
    const Expr& B,
    int offset)
{
    // B[0] -> A[offset]
    // Assume A.symbols == B.symbols
    // assume A has no transitions yet
    A.transitions[offset];
    A.transitions[offset+1];
    ClosedInterval eps;
    for (const auto& [q, dq]: B.transitions) {
        for (const auto& [a, R]: dq) {
            auto [lb, ub] = overlap_range(A.symbols, a);  // TODO factor out and use unordered_map?
            for (const auto& r: R) {
                for (auto it = lb; it != ub; it++) {
                    A.transitions[q+offset][*it].insert(r+offset);
                }
            }
        }
        // also copy epsilon-transitions if any
        if (auto it = dq.find(eps); it != dq.end()) {
            for (const auto& r: it->second) {
                A.transitions[q+offset][eps].insert(r+offset);
            }
        }
    }
}

Expr alternate(const Expr& A, const Expr& B)
{
    if (is_empty_expr(A)) {
        return B;
    } else if (is_empty_expr(B)) {
        return A;
    }
    Expr res;
    res.symbols = combine_interval_sets(A.symbols, B.symbols);
    copy_transitions(res, A, 2);
    copy_transitions(res, B, A.transitions.size() + 2);

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
    if (is_empty_expr(A) || is_empty_expr(B)) {
        return Expr();
    }
    Expr res;
    res.symbols = combine_interval_sets(A.symbols, B.symbols);
    copy_transitions(res, A, 2);
    copy_transitions(res, B, A.transitions.size() + 2);

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
