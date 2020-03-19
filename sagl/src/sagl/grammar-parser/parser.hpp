#pragma once
#include "sagl/internals/tabulate.hpp"
#include "scanner.hpp"
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace scanner {

template <class Symbol>
struct DefaultLR1Callback {
    bool accept(bool acc) { return acc; }

    using Rule = std::pair<Symbol, std::vector<Symbol>>;

    void shift(const std::pair<Symbol, std::string>& lookahead)
    {
        (void)lookahead;
    }

    void reduce(const Rule& rule)
    {
        (void)rule;
    }
};

struct Parser {
    using Symbol = typename Grammar::Symbol;
    Table table;
    const Grammar& grammar;

    Parser(const Grammar& grammar) : table(grammar), grammar(grammar)
    {}

    template <class Handler>
    auto parse(Handler& handler, std::istream& is = std::cin)
    {
        InputStack in(is);
        std::vector<int> states = {0};
        auto lookahead = scan(in);
        for (;;) {
            auto action = table.table[states.back()][lookahead.first];
            switch (action.first) {
            case Action::ACCEPT:
                return handler.accept(true);
            case Action::REDUCE: {
                const auto& rule = table.rules.value(action.second);
                handler.reduce(rule);
                for (auto it = rule.second.begin(); it != rule.second.end(); ++it) {
                    states.pop_back();
                }
                states.push_back(table.table[states.back()][rule.first].second);
                break;
             }
            case Action::SHIFT:
                states.push_back(action.second);
                handler.shift(lookahead);
                lookahead = scan(in);
                break;
            default:
                return handler.accept(false);
            }
        }
        return handler.accept(false);
    }

    bool parse()
    {
        DefaultLR1Callback<Symbol> handler;
        return parse(handler);
    }

private:
    std::pair<char const *, std::string> scan(InputStack& in)
    {
        auto [token, lexeme] = match_longest<ALL_RECOGNIZERS>(in);
        if (token == "whitespace") {
            return scan(in);
        } else if (std::string(token) == std::string(grammar.empty)) {
            uint32_t a = in.get();
            uint32_t eof = std::char_traits<char>::eof();
            if (a != eof) {
                in.unget(a);
                return std::make_pair("error", lexeme);
            }
        }
        return std::make_pair(token, lexeme);
    }
};

}