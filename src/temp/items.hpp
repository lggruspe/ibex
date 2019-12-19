#pragma once
#include <tuple>
#include <utility>
#include <vector>

template <class Symbol>
struct Item {
    using Sentence = std::vector<Symbol>;
    using Rule = std::pair<Symbol, Sentence>;

    Rule rule;
    Symbol lookahead;
    int position;

    bool is_complete() const
    {
        return position == (int)(rule.second.size());
    }

    Symbol next() const
    {
        // must not be complete
        return rule.second[position];
    }

    Sentence slice(int start = -1, int end = -1) const
    {
        start = (start == -1 ? position : start);
        end = (end == -1 ? rule.second.size() : end);
        return Sentence(rule.second.begin() + start, rule.second.begin() + end);
    }

    Item(const Rule& rule, Symbol lookahead)
        : rule(rule)
        , lookahead(lookahead)
        , position(0)
    {}

    Item shifted() const
    {
        if (is_complete()) {
            throw 0;
        }
        Item item = *this;
        item.position++;
        return item;
    }

    bool operator<(const Item& other) const
    {
        return std::tie(rule, lookahead, position) <
            std::tie(other.rule, other.lookahead, other.position);
    }
};
