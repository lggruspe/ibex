#pragma once
#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <map>
#include <set>

// TODO what should comparison return if either interval is empty?
struct SymbolInterval {
    mutable int start;
    mutable int end;

    SymbolInterval(int start, int end) : start(start), end(end) 
    {
        assert(start <= end);
    }

    SymbolInterval() : SymbolInterval(0, 0) {}
    // empty string

    // true iff not empty
    explicit operator bool() const
    {
        return start < end;
    }

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
    // symbols shouldn't contain "empty" symbols
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

void insert_symbol(std::set<SymbolInterval>& A, SymbolInterval b)
{
    assert(b);
    std::vector<SymbolInterval> B = {b};
    while (!B.empty()) {
        b = B.back();
        B.pop_back();
        auto [it, ok] = A.insert(b);
        if (!ok) {
            auto split = split_overlap(*it, b);

            assert(*it);        // symbols shouldn't have empty symbols
            assert(split[1]);   // true as long as a and b overlap
            assert(split[0] != split[2]);

            it->start = split[1].start;
            it->end = split[1].end;
            if (split[2]) {
                B.push_back(split[2]);
            }
            if (split[0]) {         // ex: [0, 1) and [0, 2)
                B.push_back(split[0]);
            }
        }
    }
}

std::set<SymbolInterval> combine_symbols(
    const std::set<SymbolInterval>& A,
    const std::set<SymbolInterval>& B)
{
    std::set<SymbolInterval> symbols = A;
    for (const auto& a: B) {
        insert_symbol(symbols, a);
    }
    return symbols;
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
    Expr res;
    res.symbols = combine_symbols(A.symbols, B.symbols);
    copy_transitions(res, A, res.symbols, 2);
    copy_transitions(res, B, res.symbols, A.transitions.size() + 2);

    int offset = A.transitions.size()+2;
    SymbolInterval eps;
    res.transitions[0][eps].insert(2);
    res.transitions[0][eps].insert(offset);
    res.transitions[3][eps].insert(1);
    res.transitions[offset+1][eps].insert(1);
    return res;
}

Expr concatenate(const Expr& A, const Expr& B)
{
    Expr res;
    res.symbols = combine_symbols(A.symbols, B.symbols);
    copy_transitions(res, A, res.symbols, 2);
    copy_transitions(res, B, res.symbols, A.transitions.size() + 2);

    int offset = A.transitions.size() + 2;
    SymbolInterval eps;
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
