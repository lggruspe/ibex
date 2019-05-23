#pragma once
#include "collection.h"
#include "enumeration.h"
#include "item.h"
#include "grammar.h"
#include <map>
#include <utility>
#include <vector>

namespace lr1
{

typedef std::pair<char, int> Action;
// first can only be s, r, a, g, or '\0' (shift, reduce, accept, goto)

template <class Symbol>
class Parser {
    std::map<int, std::map<Symbol, Action>> table;
    Enumeration<Collection<Symbol>> collections;
    std::map<int, std::map<Symbol, int>> delta;
    cfg::Grammar<Symbol> grammar;

    template<class T>
    friend void print_collections(const Parser<T>&);
    template<class T>
    friend void print_automaton(const Parser<T>&);
    template<class T>
    friend void print_table(const Parser<T>&);

    void fill_in_row(int ind, const Symbol& start) 
    {
        // compute shifts and gotos
        for (const auto& transition: delta[ind]) {
            const auto& sym = transition.first;
            int next_state = transition.second;
            table[ind][sym] = Action(
                    (grammar.is_variable(sym) ? 'g' : 's'), next_state);
        }

        // compute reductions and accepts
        for (auto item: collections.value(ind).items) {
            if (!item.is_reduce()) {
                continue;
            }
            if (item.lhs == start && item.lookahead.empty()) {
                table[ind][item.lookahead] = Action('a', 0);
            } else {
                // regular reduction
                table[ind][item.lookahead] = Action('r', grammar.rule(item.rule()));
            }
        }
    }

    void construct_automaton(const Symbol& start)
    {
        // assume the grammar is augmented
        // rebuilds automaton (delta) and collections
        delta.clear();
        collections.clear();  // acts as history
        Collection<Symbol> state;
        state.items.insert(Item<Symbol>(start, {""}, *(grammar.productions(start).begin()), ""));
        state.closure(grammar);
        std::vector<int> states = {collections.insert(state)};

        while (!states.empty()) {
            int name = states.back();
            states.pop_back();
            state = collections.value(name);
            std::map<Symbol, Collection<Symbol>> transitions = state.transition(grammar);
            for (const auto& p: transitions) {
                int pname;
                if (!collections.has_value(p.second)) {
                    pname = collections.insert(p.second);
                    states.push_back(pname);
                } else {
                    pname = collections.index(p.second);
                }
                delta[name][p.first] = pname;
            }
        }
    }

public:

    Parser(const cfg::Grammar<Symbol>& grammar) 
    {
        this->grammar = grammar;
    }

    void construct(const Symbol& start) 
    {
        construct_automaton(start);
        table.clear();
        for (const auto& state: collections) {
            fill_in_row(state.first, start);
        }
    }
};

} // end namespace
