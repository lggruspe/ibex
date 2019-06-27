#pragma once
#include <tuple>
#include <vector>

namespace sagl
{
;

template <class Symbol>
struct Rule {
    using Sentence = std::vector<Symbol>;
    Symbol lhs;
    Sentence rhs;

    Rule() {}   // should only be used by map
    Rule(Symbol lhs, Sentence rhs) : lhs(lhs), rhs(rhs) {}
    Rule(std::pair<Symbol, Sentence> rule) 
        : lhs(rule.first)
        , rhs(rule.second)
    {}

    bool operator<(const Rule& other) const
    {
        return std::tie(lhs, rhs) < std::tie(other.lhs, other.rhs);
    }
};

} // end namespace
