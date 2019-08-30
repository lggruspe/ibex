#pragma once
#include "automaton.hpp"
#include "grammar.hpp"
#include "parsingtable.hpp"
#include <algorithm>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

namespace sagl
{
;

template <class T>
using accept_callback_t = T(bool);

template <class Rule>
using reduce_callback_t = void(Rule);

template <class TokenLexemePair>
using shift_callback_t = void(TokenLexemePair);

bool empty_accept_cb(bool accept)
{
    return accept;
}

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
    template <class Scanner, 
             class T=bool,
             class U=Rule<Symbol>,
             class V=std::pair<Symbol, std::string>>
    T operator()(Scanner scan, 
            accept_callback_t<T> accept_cb = empty_accept_cb,
            reduce_callback_t<U> reduce_cb = empty_reduce_cb,
            shift_callback_t<V> shift_cb = empty_shift_cb)
    {
        std::vector<int> states = {start_state};
        auto lookahead = scan();
        for (;;) {
            auto action = table[states.back()][lookahead.first];
            switch (action.type) {
            case 'a':
                return accept_cb(true);
            case 'r': {
                const auto& rule = grammar->rule_value(action.value);
                reduce_cb(rule);
                std::for_each(rule.second.begin(), rule.second.end(),
                        [&states](const auto&) {
                        states.pop_back();
                        });
                action = table[states.back()][rule.first];
                states.push_back(action.value);
                }
                break;
            case 's':
                shift_cb(lookahead);
                states.push_back(action.value);
                lookahead = scan();
                break;
            default:
                return accept_cb(false);
            }
        }
    }
};
} // end namespace
