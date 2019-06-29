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
        const auto& state = automaton.states.value(state_id);

        // check for accept and reduce actions
        for (const auto& item: state.items) {
            if (item.reduces()) {
                // check if accept
                if (item.rule.lhs == grammar.start 
                        && item.lookahead == grammar.empty) {
                    table[state_id][grammar.empty] = Action('a');
                } else {
                    // TODO check for reduce/reduce conflicts
                    table[state_id][item.lookahead] = Action('r', item.rule_id);
                }
            }
        }

        // look for shifts and gotos
        for (const auto& [symbol, next_state_id]: 
                automaton.transitions.at(state_id)) {
            // TODO check for S/R conflicts
            table[state_id][symbol] = Action(
                    grammar.is_variable(symbol) ? 'g' : 's',
                    next_state_id);
        }
    }
};

} // end namespace
