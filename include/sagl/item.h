#pragma once
#include "rule.h"
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
    typename Sentence::iterator after;   // points to first symbol after dot

    Item(int rule_id,
            const Rule& rule,
            Symbol lookahead)
        : rule_id(rule_id)
        , rule(rule)
        , lookahead(lookahead)
    {
        after = this->rule.rhs.begin();
    }

    Item(int rule_id, 
            const Rule& rule, 
            Symbol lookahead,
            typename Sentence::iterator after)
        : rule_id(rule_id)
        , rule(rule)
        , lookahead(lookahead)
        , after(after)
    {}

    Item shifted() const
    {
        return Item(rule_id, rule, lookahead, std::next(after));
    }

    bool reduces() const
    {
        return after == rule.rhs.end();
    }

    // has special meaning when end = 0
    // (from start to lastmost position)
    std::pair<typename Sentence::iterator, 
        typename Sentence::iterator>
    after_slice(typename Sentence::size_type start, 
            typename Sentence::size_type end) const
    {
        if (!end) {
            return std::make_pair(after + start, after + rule.rhs.size());
        }
        return std::make_pair(after + start, after + end);
    }

    bool operator<(const Item<Symbol>& other) const
    {
        return std::tie(rule_id, after) < std::tie(other.rule_id, other.after);
    }
};

} // end namespace
