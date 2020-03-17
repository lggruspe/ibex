#pragma once
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

    Table(const Grammar& grammar);

    bool is_terminal(const Symbol& symbol) const
    {
        return grammar_ptr->is_terminal(symbol);
    }

private:
    void set(int q, Symbol c, Action a, int v);

    Grammar const* grammar_ptr;
};
