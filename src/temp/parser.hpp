#pragma once
#include "automaton.hpp"
#include "grammar.hpp"
#include "scanners.hpp"
#include "tabulate.hpp"
#include <string>
#include <tuple>
#include <utility>
#include <vector>

template <class Grammar>
struct Parser {
    Table<Grammar> table;

    Parser(const Grammar& grammar) : table(grammar)
    {}

    bool parse()
    {
        InputStack in;
        std::vector<int> states = {0};
        std::vector<typename Grammar::Symbol> words;
        auto [lookahead, debug] = match_longest<ALL_RECOGNIZERS>(in);
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
                std::tie(lookahead, debug) = match_longest<ALL_RECOGNIZERS>(in);
                break;
            default:
                return false;
            }
        }
        return false;
    }
};
