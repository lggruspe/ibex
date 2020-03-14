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
                // @symbol@ is replaced last, because symbol might contain
                // @...@, which could affect the other substitutions
                find_and_replace(entry, "@symbol@", symbol);
                entries.push_back(entry);
            }
        }
        std::string json = R"VOGON(
        {
            "automaton": @automaton@,
            "grammar": @grammar@
        }
        )VOGON";
        std::string automaton = "[\n";
        if (!entries.empty()) {
            automaton += entries[0];
            for (int i = 1; i < entries.size(); ++i) {
                automaton += ",\n" + entries[i];
            }
        }
        automaton += "\n]";
        find_and_replace(json, "@automaton@", automaton);

        // TODO jsonify grammar rules
        std::string grammar_json;
        for (const auto& [rule, id]: rules) {
            std::string rhs = "[";
            for (const auto& sym: rule.second) {
                rhs += "\"";
                rhs += sym;
                rhs += "\"";
                rhs += ", ";
            }
            if (rhs.back() != '[') {
                rhs.pop_back();
                rhs.pop_back();
            }
            rhs += ']';

            std::string entry = R"(
            {
                "id": @id@,
                "lhs": @lhs@,
                "rhs": @rhs@
            }
            )";
            find_and_replace(entry, "@id@", std::to_string(id));
            find_and_replace(entry, "@rhs@", rhs);
            find_and_replace(entry, "@lhs@", "\"" + std::string(rule.first) + "\"");
            if (!grammar_json.empty()) {
                grammar_json += ",\n";
            }
            grammar_json += entry;
        }
        grammar_json = "[" + grammar_json + "]";
        find_and_replace(json, "@grammar@", grammar_json);
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
