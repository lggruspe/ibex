#pragma once
#include "enumeration.hpp"
#include "grammar.hpp"
#include "item.hpp"
#include "state.hpp"
#include <map>
#include <utility>
#include <vector>

namespace sagl
{

template <class Symbol>
struct Automaton {
    std::map<int, std::map<Symbol, int>> transitions;
    int start;

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

    State<Symbol> state_value(int state_id) const
    {
        return states.value(state_id);
    }

    int state_index(const State<Symbol>& state) const
    {
        return states.index(state);
    }

private:
    Enumeration<State<Symbol>> states;
    Item<Symbol> start_item(const Grammar<Symbol>& grammar) const
    {
        using Sentence = std::vector<Symbol>;
        auto [_start, _] = grammar.rules_for(grammar.start);
        std::pair<Symbol, Sentence> rule = {_start->first, _start->second};
        return Item(grammar.rule_index(rule), rule, grammar.empty);
    }
};

} // end namespace
