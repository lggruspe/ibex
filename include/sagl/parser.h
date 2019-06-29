#pragma once
#include "automaton.h"
#include "grammar.h"
#include "parsingtable.h"
#include <algorithm>
#include <map>
#include <stdexcept>
#include <string>
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
    // scan returns a token, string pair
    // assume tokens can be implicitly converted to symbols
    template <class Scanner>
    bool operator()(Scanner scan) 
    {
        std::vector<int> states = {start_state};
        std::vector<Symbol> symbols;
        auto token_lexeme_pair = scan();
        Symbol lookahead = token_lexeme_pair.first;
        std::string lexeme = token_lexeme_pair.second;

        for (;;) {
            auto state = states.back();
            auto action = table[state][lookahead];

            switch (action.type) {
            case 'a':
                return true;
            case 'r': {
                const auto& rule = grammar->rule_value(action.value);
                std::for_each(rule.rhs.begin(), rule.rhs.end(),
                        [&states, &symbols](const auto&) {
                        states.pop_back();
                        symbols.pop_back();
                        });
                symbols.push_back(rule.lhs);
                action = table[state][rule.lhs];
                states.push_back(action.value);
                }
                break;
            case 's':
                states.push_back(action.value);
                symbols.push_back(lookahead);
                token_lexeme_pair = scan();
                lookahead = token_lexeme_pair.first;
                lexeme = token_lexeme_pair.second;

                break;
            default:
                return false;
            }
        }
    }
};

}
