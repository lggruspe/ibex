#pragma once
#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <map>
#include <set>

struct SymbolInterval {
    int start;
    int end;

    SymbolInterval(int start, int end) : start(start), end(end) 
    {
        assert(start <= end);
    }

    SymbolInterval() : SymbolInterval(0, 0) {}
    // empty string

    bool operator<(const SymbolInterval& other) const
    {
        return end <= other.start;
    }

    bool operator>(const SymbolInterval& other) const
    {
        return start >= other.end;
    }

    bool operator!=(const SymbolInterval& other) const
    {
        return (*this < other) || (*this > other);
    }

    bool operator==(const SymbolInterval& other) const
    {
        // checks if this and other overlap
        return !(*this != other);
    }
};

struct Expr {
    // invariant: start = 0, accept = 1
    std::set<SymbolInterval> symbols;
    std::map<int, std::map<SymbolInterval, std::set<int>>> transitions;

    Expr()
    {
        transitions[0];
        transitions[1];
    }

    Expr(SymbolInterval a) : symbols({a})
    {
        transitions[0][a].insert(1);
        transitions[1];
    }
};

Expr closure(const Expr& expr)
{
    Expr res;
    for (const auto& [q, dq]: expr.transitions) {
        for (const auto& [a, R]: dq) {
            for (const auto& r: R) {
                res.transitions[q+2][a].insert(r+2);
            }
        }
    }
    SymbolInterval eps;
    res.symbols.insert(eps);
    res.transitions[0][eps].insert(1);
    res.transitions[0][eps].insert(2);
    res.transitions[3][eps].insert(1);
    res.transitions[3][eps].insert(2);
    return res;
}

std::array<SymbolInterval, 3> split_overlap(
    const SymbolInterval& a, 
    const SymbolInterval& b)
{
    int endpoints[4] = {a.start, a.end, b.start, b.end};
    std::sort(endpoints, endpoints+4);
    return std::array<SymbolInterval, 3>{ 
        SymbolInterval(endpoints[0], endpoints[1]),
        SymbolInterval(endpoints[1], endpoints[2]),
        SymbolInterval(endpoints[2], endpoints[3]),
    };
}

void combine_symbols(std::set<SymbolInterval>& A, SymbolInterval b)
{
    std::set<SymbolInterval> B = {b};
    while (!B.empty()) {
        b = *(B.begin());
        B.erase(B.begin());
        auto [it, ok] = A.insert(b);
        if (!ok) {
            auto split = split_overlap(*it, b);
            A.erase(it);          // or *it = split[1]?
            A.insert(split[1]); //
            combine_symbols(B, split[0]);
            combine_symbols(B, split[2]);
        }
    }
}

void copy_transitions(
    Expr& A,
    const Expr& B,
    const std::set<SymbolInterval>& symbols, 
    int offset)
{
    A.transitions[offset];
    A.transitions[offset+1];
    for (const auto& [q, dq]: B.transitions) {
        for (const auto& [a, R]: dq) {
            for (const auto& r: R) {
                //[lb, ub) contains all intervals that overlap with a
                auto lb = symbols.lower_bound(a);   // what if symbols still has an even lower bound?
                auto ub = symbols.upper_bound(a);
                for (auto it = lb; it != ub; it++) {
                    A.transitions[q+offset][*it].insert(r+offset);
                }
            }
        }
    }
}

Expr alternate(const Expr& A, const Expr& B)
{
    std::set<SymbolInterval> symbols = A.symbols;
    for (const auto& a: B.symbols) {
        combine_symbols(symbols, a);
    }
    Expr res;
    res.symbols = symbols;
    copy_transitions(res, A, symbols, 2);
    copy_transitions(res, B, symbols, A.transitions.size() + 2);

    int offset = A.transitions.size()+2;
    SymbolInterval eps;
    res.symbols.insert(eps);
    res.transitions[0][eps].insert(2);
    res.transitions[0][eps].insert(offset);
    res.transitions[3][eps].insert(1);
    res.transitions[offset+1][eps].insert(1);
    return res;
}

Expr concatenate(const Expr& A, const Expr& B)
{
    std::set<SymbolInterval> symbols = A.symbols;
    for (const auto& a: B.symbols) {
        combine_symbols(symbols, a);
    }
    Expr res;
    res.symbols = symbols;
    copy_transitions(res, A, symbols, 2);
    copy_transitions(res, B, symbols, A.transitions.size() + 2);

    int offset = A.transitions.size() + 2;
    SymbolInterval eps;
    res.symbols.insert(eps);
    res.transitions[0][eps].insert(2);
    res.transitions[3][eps].insert(offset);
    res.transitions[offset+1][eps].insert(1);
    return res;
}

std::ostream& operator<<(std::ostream& os, const SymbolInterval& sym)
{
    return os << "[" << sym.start << ", " << sym.end << ")";
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
