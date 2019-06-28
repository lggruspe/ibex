#pragma once
#include "rule.h"
#include <iostream>
#include <iterator>
#include <tuple>
#include <utility>
#include <vector>

namespace sagl
{
;

template <class Symbol>
struct Item {
    using Sentence = std::vector<Symbol>;
    using Rule = Rule<Symbol>;

    int rule_id;    // TODO should match type of rule_id in grammar
    Rule rule;
    Symbol lookahead;

    Item(int rule_id, 
            const Rule& rule, 
            Symbol lookahead,
            typename Sentence::size_type dot_index=0)
        : rule_id(rule_id)
        , rule(rule)
        , lookahead(lookahead)
        , dot_index(dot_index)
    {}

    Item shifted() const
    {
        return Item(rule_id, rule, lookahead, dot_index+1);
    }

    bool reduces() const
    {
        return dot_index == rule.rhs.size();
    }

    // symbol after the dot
    typename Sentence::const_iterator after() const
    {
        
        return rule.rhs.cbegin() + dot_index;
    }

    // has special meaning when end = 0
    // (from start to lastmost position)
    std::pair<typename Sentence::const_iterator, 
        typename Sentence::const_iterator>
    after_slice(typename Sentence::size_type start, 
            typename Sentence::size_type end) const
    {
        return std::make_pair(rule.rhs.cbegin() + dot_index + start,
                end ? rule.rhs.cbegin() + dot_index + end
                    : rule.rhs.cend());
    }

    bool operator<(const Item<Symbol>& other) const
    {
        return std::tie(rule_id, dot_index) 
            < std::tie(other.rule_id, other.dot_index);
    }

private:
    typename Sentence::size_type dot_index;   // of first symbol after dot
};

template <class Symbol>
std::ostream& operator<<(std::ostream& out, const Item<Symbol>& item)
{
    out << "{" << item.rule.lhs << " -> ";
    for (auto i = 0; i < item.rule.rhs.size(); ++i) {
        if (i == item.dot_index) {
            out << ". ";
        }
        out << item.rule.rhs[i] << " ";
    }
    out << "}";
    return out;
}

} // end namespace
