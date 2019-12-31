#pragma once
#include "sagl/tabulate.hpp"
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "../scan/scanner.hpp"

template <class Grammar>
struct Parser {
    using Symbol = typename Grammar::Symbol;
    Table<Grammar> table;
    const Grammar& grammar;

    Parser(const Grammar& grammar) : table(grammar), grammar(grammar)
    {}

    bool is_valid()
    {
        scanner::InputStack in;
        std::vector<int> states = {0};
        std::vector<Symbol> words;
        auto [lookahead, _] = scan(in);
        for (;;) {
            auto action = table.table[states.back()][lookahead];
            switch (action.first) {
            case Action::ACCEPT:
                return true;
            case Action::REDUCE: {
                const auto& rule = table.rules.value(action.second);
                for (auto it = rule.second.begin(); it != rule.second.end(); ++it) {
                    states.pop_back();
                    words.pop_back();
                }
                words.push_back(rule.first);
                states.push_back(table.table[states.back()][rule.first].second);
                break;
             }
            case Action::SHIFT:
                states.push_back(action.second);
                words.push_back(lookahead);
                std::tie(lookahead, _) = scan(in);
                break;
            default:
                return false;
            }
        }
        return false;
    }

private:
    std::pair<Symbol, std::string> scan(scanner::InputStack& in)
    {
        auto [token, lexeme] = scanner::match_longest<ALL_RECOGNIZERS>(in);
        if (Symbol(token) == grammar.empty) {
            uint32_t a = in.get();
            uint32_t eof = std::char_traits<char>::eof();
            if (a != eof) {
                in.unget(a);
                return std::make_pair(Symbol(Variable::ERROR), lexeme);
            }
        }
        return std::make_pair(Symbol(token), lexeme);
    }
};
