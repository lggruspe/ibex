#pragma once
#include "automaton.hpp"
#include "grammar.hpp"
#include "handles.hpp"
#include <iostream>
#include <map>
#include <utility>
#include <vector>

enum class Action { ERROR = 0, ACCEPT, GOTO, REDUCE, SHIFT };

class ShiftReduceConflict {};

class ReduceReduceConflict {};

template <class Grammar>
struct Table {
    // NOTE empty entry means error
    using Symbol = typename Grammar::Symbol;
    std::map<int, std::map<Symbol, std::pair<Action, int>>> table;
    HandleSet<typename Grammar::Rule> rules;

    Table(const Grammar& grammar) : grammar_ptr(&grammar)
    {
        for (const auto& rule: grammar.rules) {
            rules.insert(rule);
        }
        Automaton m(grammar);
        for (const auto& [state, p]: m) {
            // set shifts and gotos
            for (const auto& [a, q]: m.transitions[p]) {
                if (grammar.is_terminal(a)) {
                    set(p, a, Action::SHIFT, q);
                } else {
                    set(p, a, Action::GOTO, q);
                }
            }
            // set accepts and reductions
            for (const auto& item: state) {
                if (item.is_complete()) {
                    if (item.rule.first == grammar.start && item.lookahead == Symbol(grammar.empty)) {
                        set(p, item.lookahead, Action::ACCEPT);
                    } else {
                        set(p, item.lookahead, Action::REDUCE, rules.index(item.rule));
                    }
                }
            }
        }
    }

    std::string jsonify() const
    {
        std::string json;
        json += "[\n";
        std::vector<std::string> entries;
        for (const auto& [state, actions]: table) {
            for (const auto& [symbol, action]: actions) {
                std::string entry = R"VOGON(
                {
                    "state": @state@,
                    "symbol": "@symbol@",
                    "terminal": @terminal@,
                    "type": "@type@",
                    "action": @action@
                })VOGON";
                find_and_replace(entry, "@state@", std::to_string(state));
                find_and_replace(entry, "@symbol@", symbol);
                find_and_replace(entry, "@terminal@", grammar_ptr->is_terminal(symbol) ? "true" : "false");
                find_and_replace(entry, "@type@",
                        action.first == Action::ERROR ? "error" :
                        action.first == Action::ACCEPT ? "accept" :
                        action.first == Action::GOTO ? "goto" :
                        action.first == Action::REDUCE ? "reduce" :
                        action.first == Action::SHIFT ? "shift"
                            : "error");
                find_and_replace(entry, "@action@",
                        action.first == Action::ERROR ? "0" : std::to_string(action.second));
                entries.push_back(entry);
            }
        }
        if (!entries.empty()) {
            json += entries[0];
            for (int i = 1; i < entries.size(); ++i) {
                json += ",\n" + entries[i];
            }
        }
        json += "\n]";
        return json;
    }

private:
    void find_and_replace(
            std::string& text,
            const std::string& before,
            const std::string& after) const
    {
        auto pos = text.find(before);
        if (pos != std::string::npos) {
            text.replace(pos, before.length(), after);
        }
    }

    void set(int q, Symbol c, Action a, int v = 0)
    {
        auto& entry = table[q][c];
        if (entry != std::make_pair(Action::ERROR, 0)) {
            if (entry.first == a) {
                throw ReduceReduceConflict();
            }
            throw ShiftReduceConflict();
        }
        entry = std::make_pair(a, v);
    }

    Grammar const* grammar_ptr;
};
