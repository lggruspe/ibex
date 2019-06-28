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
    typename Sentence::iterator after;   // points to first symbol after dot

    Item(int rule_id,
            const Rule& rule,
            Symbol lookahead)
        : rule_id(rule_id)
        , rule(rule)
        , lookahead(lookahead)
    {
        after = this->rule.rhs.begin();
        rhs_end = this->rule.rhs.end();
    }

    Item(int rule_id, 
            const Rule& rule, 
            Symbol lookahead,
            typename Sentence::iterator after)
        : rule_id(rule_id)
        , rule(rule)
        , lookahead(lookahead)
        , after(after)
    {
        rhs_end = this->rule.rhs.end();
    }

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
            return std::make_pair(after + start, rhs_end);
        }
        return std::make_pair(after + start, after + end);
    }

    bool operator<(const Item<Symbol>& other) const
    {
        return std::tie(rule_id, after) < std::tie(other.rule_id, other.after);
    }

private:
    typename Sentence::iterator rhs_end; // used to keep after_slice const
};

template <class Symbol>
std::ostream& operator<<(std::ostream& out, const Item<Symbol>& item)
{
    out << "{" << item.rule.lhs << " -> ";
    for (auto it = item.rule.rhs.begin(); it != item.rule.rhs.end(); ++it) {
        if (it == item.after) {
            out << ". ";
        }
        out << *it << " ";
    }
    out << "}";
    return out;
}

} // end namespace
