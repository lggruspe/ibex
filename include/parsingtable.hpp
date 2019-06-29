#pragma once
#include "automaton.hpp"
#include "grammar.hpp"
#include <iostream>
#include <map>

namespace sagl
{
;

struct Action {
    char type;
    int value;

    // e: error
    // s: shift
    // g: goto
    // r: reduce
    // a: accept
    Action(char type='e', int value=0) : type(type), value(value) {}
};

std::ostream& operator<<(std::ostream& out, const Action& action)
{
    return out << action.type << action.value;
}
template <class Symbol>
struct ParsingTable {
    std::map<int, std::map<Symbol, Action>> table;
    ParsingTable(const Automaton<Symbol>& automaton, 
            const Grammar<Symbol>& grammar)
    {
        for (auto [state_id, _]: automaton.transitions) {
            fill_in_row(state_id, automaton, grammar);
        }
    }

private:
    void fill_in_row(int state_id, 
            const Automaton<Symbol>& automaton, 
            const Grammar<Symbol>& grammar)
    {
        const auto& state = automaton.state_value(state_id);

        // check for accept and reduce actions
        for (const auto& item: state.items) {
            if (item.reduces()) {
                // check if accept
                if (item.rule.lhs == grammar.start 
                        && item.lookahead == grammar.empty) {
                    table[state_id][grammar.empty] = Action('a');
                } else {
                    auto& action = table[state_id][item.lookahead];
                    if (action.type == 'r' && action.value != item.rule_id) {
                        std::cerr << "Warning: reduce/reduce conflict\n";
                    }
                    action = Action('r', item.rule_id);
                }
            }
        }

        // look for shifts and gotos
        for (const auto& [symbol, next_state_id]: 
                automaton.transitions.at(state_id)) {
            auto& action = table[state_id][symbol];
            if (action.type == 'r') {
                std::cerr << "Warning: shift/reduce conflict\n";
            }
            action = Action(
                    grammar.is_variable(symbol) ? 'g' : 's',
                    next_state_id);
        }
    }
};

} // end namespace
