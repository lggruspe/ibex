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

using accept_callback = void();

template <class Rule>
using reduce_callback = void(Rule);

template <class TokenLexemePair>
using shift_callback = void(TokenLexemePair);

void empty_accept_cb() {}

template <class Rule>
void empty_reduce_cb(Rule rule) {}

template <class TokenLexemePair>
void empty_shift_cb(TokenLexemePair lookahead) {}

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

    // scan returns a token, string pair
    // assume tokens can be implicitly converted to symbols
    template <class Scanner, class T=Rule<Symbol>, class U=std::pair<Symbol, std::string>>
    bool operator()(Scanner scan, 
            accept_callback accept_cb = empty_accept_cb,
            reduce_callback<T> reduce_cb = empty_reduce_cb,
            shift_callback<U> shift_cb = empty_shift_cb)
    {
        std::vector<int> states = {start_state};
        auto lookahead = scan();
        for (;;) {
            auto state = states.back();
            auto action = table[state][lookahead.first];

            switch (action.type) {
            case 'a':
                accept_cb();
                return true;
            case 'r': {
                const auto& rule = grammar->rule_value(action.value);
                reduce_cb(rule);
                std::for_each(rule.rhs.begin(), rule.rhs.end(),
                        [&states](const auto&) {
                        states.pop_back();
                        });
                action = table[state][rule.lhs];
                states.push_back(action.value);
                }
                break;
            case 's':
                shift_cb(lookahead);
                states.push_back(action.value);
                lookahead = scan();
                break;
            default:
                return false;
            }
        }
    }
};

} // end namespace
