#pragma once
#include "automaton.hpp"
#include "grammar.hpp"
#include "parsingtable.hpp"
#include <algorithm>
#include <map>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

namespace sagl
{

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
        table = make_parse_table(grammar, automaton);
        start_state = automaton.start;
    }

    // scan returns a token, string pair
    // assume tokens can be implicitly converted to symbols
    template <class Scanner, class T = bool>
    T operator()(Scanner scan, 
            accept_callback_t<T> accept_cb = empty_accept_cb,
            reduce_callback_t<std::pair<Symbol, std::vector<Symbol>>> reduce_cb = empty_reduce_cb,
            shift_callback_t<std::pair<Symbol, std::string>> shift_cb = empty_shift_cb)
    {
        std::vector<int> states = {start_state};
        auto [token, lexeme] = scan();
        for (;;) {
            auto action = table[states.back()][token];
            switch (action.type) {
            case 'a':
                return accept_cb(true);
            case 'r': {
                const auto& [lhs, rhs] = grammar->rule_value(action.value);
                reduce_cb({lhs, rhs});
                std::for_each(rhs.begin(), rhs.end(),
                        [&states](const auto&) {
                            states.pop_back();
                        });
                action = table[states.back()][lhs];
                states.push_back(action.value);
                }
                break;
            case 's':
                shift_cb({token, lexeme});
                states.push_back(action.value);
                std::tie(token, lexeme) = scan();
                break;
            default:
                return accept_cb(false);
            }
        }
    }
};

} // end namespace
