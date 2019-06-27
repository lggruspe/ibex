#pragma once
#include "item.h"
#include "grammar.h"
#include <initializer_list>
#include <map>
#include <set>
#include <vector>

namespace sagl
{
;

template <class Symbol>
struct State {
    std::set<Item<Symbol>> items;

    State() {}
    State(std::initializer_list<Item<Symbol>> items, 
            const Grammar<Symbol>& grammar) : items(items)
    {
        closure(grammar);
    }

    void closure(const Grammar<Symbol>& grammar)
    {
        std::vector<Item<Symbol>> queue(items.begin(), items.end());
        while (!queue.empty()) {
            auto item = queue.back();
            queue.pop_back();
            if (!item.reduces()) {
                auto lhs = *(item.after);
                auto [start, end] = grammar.rules_for(lhs);
                for (auto it = start; it != end; ++it) {
                    auto [_start, _end] = item.after_slice(1, 0);
                    std::vector<Symbol> sentence(_start, _end);
                    sentence.push_back(item.lookahead);
                    auto new_lookaheads = grammar.first(sentence);

                    const auto& rule = *it;
                    int rule_id = grammar.rule_index(rule);
                    for (const auto& new_lookahead: new_lookaheads) {
                        Item<Symbol> new_item(rule_id, rule, new_lookahead);
                        items.insert(new_item);
                    }
                }
            }
        }
    }

    std::map<Symbol, State> transitions(
            const Grammar<Symbol>& grammar) const {
        std::map<Symbol, State> neighbors;
        for (const auto& item: items) {
            if (!item.reduces()) {
                const auto& lhs = *(item.after);
                neighbors[lhs].insert(item.shifted());
            }
        }

        for (auto [_, neighbor]: neighbors) {
            neighbor.closure(grammar);
        }
        return neighbors;
    }

    bool operator<(const State<Symbol>& other) const
    {
        return items < other.items;
    }

private:
    void insert(const Item<Symbol>& item)
    {
        items.insert(item);
    }
};

} // end namespace
