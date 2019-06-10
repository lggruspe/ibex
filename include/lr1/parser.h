#pragma once
#include "collection.h"
#include "enumeration.h"
#include "grammar.h"
#include <algorithm>
#include <map>
#include <utility>
#include <vector>
#include <iostream>
   
namespace lr1
{

template <class Token, class Variable>
struct RulesTable {
    // wrapper for Enumeration<Rule>
    using Symbol = std::variant<Token, Variable>;
    using Sentence = typename std::vector<Symbol>;
    using Rule = typename std::pair<Variable, Sentence>;
    Enumeration<Rule> rules;
    Symbol empty;

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

template <class Token>
void _default_shifter(const Token&, const std::string&) {}

template <class Token, class Variable>
void _default_reducer(const std::pair<Variable, std::vector<std::variant<Token, Variable>>>&) {}

template <class Token, class Variable>
class Parser {
    using Symbol = std::variant<Token, Variable>;
    std::map<int, std::map<Symbol, std::pair<Action, int>>> table;
    Enumeration<Collection<Token, Variable>> collections;
    std::map<int, std::map<Symbol, int>> delta;
    cfg::Grammar<Token, Variable> grammar;
    RulesTable<Token, Variable> rules;

    void fill_in_row(int ind, const Symbol& start) 
    {
        // compute shifts and gotos
        for (const auto& [sym, next_state]: delta[ind]) {
            table[ind][sym] = { 
                cfg::is_variable(sym) ? Action::Goto : Action::Shift,
                next_state
            };
        }

        // compute reductions and accepts
        for (const auto& item: collections.value(ind).items) {
            if (!item.is_reduce()) {
                continue;
            }
            if (item.lhs == std::get<Variable>(start) 
                    && std::get<Token>(item.lookahead) == grammar.empty) {
                table[ind][item.lookahead] = { Action::Accept, 0 };
            } else {
                // regular reduction
                table[ind][item.lookahead] = { 
                    Action::Reduce, 
                    rules.index(item) 
                };
            }
        }
    }

    void construct_automaton(const Variable& start)
    {
        // assume the grammar is augmented
        // rebuilds automaton (delta) and collections
        delta.clear();
        collections.clear();  // acts as history
        Collection<Token, Variable> state;
        state.items.insert(Item<Token, Variable>(start,
                    {grammar.empty}, *(grammar.rules[start].begin()), 
                    grammar.empty));

        state.closure(grammar);
        std::vector<int> states = {collections.insert(state)};

        while (!states.empty()) {
            int id = states.back();
            states.pop_back();
            state = collections.value(id);
            std::map<Symbol, Collection<Token, Variable>> transitions = state.transition(grammar);
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

    void construct(const Variable& start)
    {
        construct_automaton(start);
        table.clear();
        for (const auto& state: collections) {
            fill_in_row(state.first, start);
        }
    }

    void construct(const Symbol& start) 
    {
        if (!cfg::is_variable(start)) {
            throw std::invalid_argument("start symbol must be a variable");
        }
        construct(std::get<Variable>(start));
    }

    template <class Scanner>
    // a scanner is a functor/function that returns a token-lexeme pair
    bool parse(Scanner scan, void shifter(const Token&, const std::string&),
            void reducer(const std::pair<Variable, std::vector<std::variant<Token, Variable>>>&))
    {
        // assumes 0 is the start state
        std::vector<int> states = {0};
        std::vector<Symbol> symbols;
        auto lookahead = scan();
        for (;;) {
            int state = states.back();
            auto [action, next_state] = table[state][lookahead.first];

            if (action == Action::Accept) {
                return true;
            } else if (action == Action::Shift) {
                shifter(lookahead.first, lookahead.second);
                states.push_back(next_state);
                symbols.push_back(lookahead.first);
                lookahead = scan();
            } else if (action == Action::Reduce) {
                auto rule = rules.value(next_state);
                reducer(rule);
                std::for_each(rule.second.begin(), rule.second.end(),
                        [&states, &symbols](const auto&) {
                            states.pop_back();
                            symbols.pop_back();
                        });
                symbols.push_back(rule.first);
                states.push_back(table[states.back()][rule.first].second);
            } else {
                return false;
            }
        }
    }

    template <class Scanner>
    bool parse(Scanner scan)
    {
        return parse(scan, _default_shifter, _default_reducer);
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

    void _trace(const std::vector<int>& states, const std::vector<Symbol>& symbols,
            const Symbol& lookahead) const
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
