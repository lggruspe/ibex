#pragma once
#include "enumeration.h"
#include "grammar.h"
#include "item.h"
#include "rule.h"
#include "state.h"
#include <map>
#include <vector>

namespace sagl
{
;

template <class Symbol>
struct Automaton {
    // TODO hide states enumeration and provide methods 
    // for converting between state_ids and states
    Enumeration<State<Symbol>> states;
    std::map<int, std::map<Symbol, int>> transitions;
    int start;  // TODO type must match index type in enumeration.h

    Automaton(const Grammar<Symbol>& grammar)
    {
        State<Symbol> state({start_item(grammar)}, grammar);
        start = states.insert(state);
        std::vector<int> queue = {start};
        while (!queue.empty()) {
            auto state_id = queue.back();
            queue.pop_back();
            state = states.value(state_id);
            auto neighbors = state.transitions(grammar);
            for (const auto& [symbol, neighbor]: neighbors) {
                bool push = !states.has_value(neighbor);
                int neighbor_id = states.insert(neighbor);
                transitions[state_id][symbol] = neighbor_id;
                transitions[neighbor_id];
                if (push) {
                    queue.push_back(neighbor_id);
                }
            }
        }
    }

private:
    Item<Symbol> start_item(const Grammar<Symbol>& grammar) const
    {
        auto [_start, _] = grammar.rules_for(grammar.start);
        const Rule<Symbol>& rule = *_start;
        return Item(grammar.rule_index(rule), rule, grammar.empty);
    }
};

} // end namespace
