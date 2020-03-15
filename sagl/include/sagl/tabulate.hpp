#pragma once
#include "automaton.hpp"
#include "grammar.hpp"
#include "handles.hpp"
#include <map>
#include <utility>

enum class Action { ERROR = 0, ACCEPT, GOTO, REDUCE, SHIFT };

class ShiftReduceConflict {};

class ReduceReduceConflict {};

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

    bool is_terminal(const std::string& symbol) const
    {
        return grammar_ptr->is_terminal(symbol);
    }

private:

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
