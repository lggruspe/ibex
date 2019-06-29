#pragma once
#include "automaton.h"
#include "grammar.h"
#include "parsingtable.h"
#include <map>
#include <vector>

namespace sagl
{
;

template <class Symbol>
class Parser {
    std::map<int, std::map<Symbol, Action>> table;
    int start_state;
    Grammar<Symbol>* grammar;

public:
    Parser(Grammar<Symbol>& grammar) : grammar(&grammar)
    {
        Automaton automaton(grammar);
        ParsingTable parsing_table(automaton, grammar);
        table = parsing_table.table;
        start_state = automaton.start;
    }

    // TODO pass shift, reduce, accept and goto callbacks
    template <class Scanner>
    bool operator()(Scanner scan) const
    {
        std::vector<int> states = {start_state};
        std::vector<Symbol> symbols;
        Symbol lookahead = scan();

        for (;;) {
            auto state = states.back();
            auto action = table.at(state).at(lookahead);

            switch (action.type) {
            case 'a':
                return true;
            case 'r': {
                const auto& rule = grammar->rule_value(action.value);
                for (const auto& _: rule.rhs) {
                    states.pop_back();
                    symbols.pop_back();
                }
                symbols.push_back(rule.lhs);
                states.push_back(table.at(state).at(rule.lhs));
                break;
            }
            case 's':
                states.push_back(action.value);
                symbols.push_back(lookahead);
                lookahead = scan();
                break;
            default:
                return false;
            }
        }
    }
};

}
