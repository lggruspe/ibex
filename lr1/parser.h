#pragma once
#include "collection.h"
#include "enumeration.h"
#include "grammar.h"
#include <map>
#include <utility>
#include <vector>
#include <iostream>
   
namespace lr1
{

template <class Token, class Variable>
struct RulesTable {
    // wrapper for Enumeration<Rule>
    using Sym = cfg::Symbol<Token, Variable>;
    using Sentence = typename std::vector<Sym>;
    using Rule = typename std::pair<Variable, Sentence>;
    Enumeration<Rule> rules;
    Token empty;

    RulesTable(const cfg::Grammar<Token, Variable>& grammar)
    {
        for (const auto& [var, prods]: grammar.rules) {
            for (const auto& rhs: prods) {
                rules.insert({var, rhs});
            }
        }
        empty = grammar.empty;
    }

    int index(const Rule& rule) const
    {
        return rules.index(rule);
    }

    int index(const Item<Token, Variable>& item) const
    {
        // construct rule
        Sentence rhs;
        std::remove_copy(item.before.begin(), item.before.end(), 
                std::back_inserter(rhs), empty);
        std::remove_copy(item.after.begin(), item.after.end(), 
                std::back_inserter(rhs), empty);
        return index({item.lhs, rhs});
    }

    const Rule& value(int ind) 
    {
        return rules.value(ind);
    }
};

enum class Action { Error, Shift, Reduce, Accept, Goto };

template <class Token, class Variable>
class Parser {
    using Sym = cfg::Symbol<Token, Variable>;
    std::map<int, std::map<Sym, std::pair<Action, int>>> table;
    Enumeration<Collection<Token, Variable>> collections;
    std::map<int, std::map<Sym, int>> delta;
    cfg::Grammar<Token, Variable> grammar;
    RulesTable<Token, Variable> rules;

    void fill_in_row(int ind, const Sym& start) 
    {
        // compute shifts and gotos
        for (const auto& [sym, next_state]: delta[ind]) {
            table[ind][sym] = { 
                sym.is_variable() ? Action::Goto : Action::Shift,
                next_state
            };
        }

        // compute reductions and accepts
        for (const auto& item: collections.value(ind).items) {
            if (!item.is_reduce()) {
                continue;
            }
            if (item.lhs == start.variable() && item.lookahead == grammar.empty) {
                table[ind][item.lookahead] = { Action::Accept, 0 };
            } else {
                // regular reduction
                table[ind][item.lookahead] = { Action::Reduce, rules.index(item) };
            }
        }
    }

    void construct_automaton(const Sym& start)
    {
        // assume the grammar is augmented
        // rebuilds automaton (delta) and collections
        delta.clear();
        collections.clear();  // acts as history
        Collection<Token, Variable> state;
        state.items.insert(Item<Token, Variable>(start.variable(), 
                    {grammar.empty}, *(grammar.rules[start.variable()].begin()), 
                    grammar.empty));

        state.closure(grammar);
        std::vector<int> states = {collections.insert(state)};

        while (!states.empty()) {
            int id = states.back();
            states.pop_back();
            state = collections.value(id);
            std::map<Sym, Collection<Token, Variable>> transitions = state.transition(grammar);
            for (const auto& [sym, next_state]: transitions) {
                int sid;
                if (!collections.has_value(next_state)) {
                    sid = collections.insert(next_state);
                    states.push_back(sid);
                } else {
                    sid = collections.index(next_state);
                }
                delta[id][sym] = sid;
            }
        }
    }

public:
    Parser(const cfg::Grammar<Token, Variable>& grammar) : 
        grammar(grammar), rules(grammar) {}

    void construct(const Sym& start) 
    {
        if (!start.is_variable()) {
            throw std::invalid_argument("start symbol must be a variable");
        }
        construct_automaton(start);
        table.clear();
        for (const auto& state: collections) {
            fill_in_row(state.first, start);
        }
    }

    template <class Scanner>
    // a scanner is a functor/function that returns a token-lexeme pair
    bool parse(Scanner scan)
    {
        // assumes 0 is the start state
        std::vector<int> states = {0};
        std::vector<Sym> symbols;
        Sym lookahead = scan().first;
        for (;;) {
            int state = states.back();
            auto [action, next_state] = table[state][lookahead];

            if (action == Action::Accept) {
                return true;
            } else if (action == Action::Shift) {
                states.push_back(next_state);
                symbols.push_back(lookahead);
                lookahead = scan().first;
            } else if (action == Action::Reduce) {
                auto rule = rules.value(next_state);
                for (const auto& _: rule.second) {
                    states.pop_back();
                    symbols.pop_back();
                }
                symbols.push_back(rule.first);
                states.push_back(table[states.back()][rule.first].second);
            } else {
                return false;
            }
        }
    }

private:
    // for printing parse trace
    template <class ValueType>
    void _print_vector(const std::vector<ValueType>& vec) const
    {
        // assume members are printable
        for (const auto& x: vec) {
            std::cout << x << ' ';
        }
        std::cout << std::endl;
    }

    void _trace(const std::vector<int>& states, const std::vector<Sym>& symbols,
            const Sym& lookahead) const
    {
        std::cout << "states: ";
        _print_vector(states);
        std::cout << "symbols: ";
        _print_vector(symbols);
        std::cout << "lookahead: " << lookahead << std::endl;
        std::cout << std::endl;
    }

    template<class T, class U>
    friend void print_collections(const Parser<T, U>&);
    template<class T, class U>
    friend void print_automaton(const Parser<T, U>&);
    template<class T, class U>
    friend void print_table(const Parser<T, U>&);
};

} // end namespace
