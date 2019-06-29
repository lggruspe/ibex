#pragma once
#include "enumeration.h"
#include "rule.h"
#include <algorithm>
#include <cassert>
#include <initializer_list>
#include <map>
#include <set>
#include <utility>
#include <vector>

#include <iostream>

namespace sagl
{
;


template <class Symbol>
struct Grammar {
    using Sentence = std::vector<Symbol>;

    std::set<Symbol> symbols;
    Symbol start;
    Symbol empty;
   
    Grammar(Symbol start, Symbol empty, 
            std::initializer_list<Rule<Symbol>> rules)
    : start(start)
    , empty(empty)
    , rules(rules)
    {
        symbols.insert(start);
        symbols.insert(empty);
        for (const auto& rule: this->rules) {
            rules_table.insert(rule);
            symbols.insert(rule.lhs);
            symbols.insert(rule.rhs.begin(), rule.rhs.end());
        }
        compute_first_sets();
    }

    std::set<Symbol> first(Sentence sentence) const
    {
        std::set<Symbol> res;
        for (const auto& sym: sentence) {
            const auto& first_set = first(sym);
            res.insert(first_set.begin(), first_set.end());
            if (!first_set.count(empty)) {
                break;
            }
        }
        return res;
    }

    std::pair<typename std::set<Rule<Symbol>>::iterator,
        typename std::set<Rule<Symbol>>::iterator>
    rules_for(Symbol sym) const
    {
        auto lb = rules.lower_bound({sym, {}});
        auto ub = std::find_if(lb, rules.end(),
                [&sym](const auto& rule) {
                    return rule.lhs != sym;
                });
        return std::make_pair(lb, ub);
    }

    // TODO check if enumeration.index returns an int or something else
    int rule_index(const Rule<Symbol>& rule) const
    {
        return rules_table.index(rule);
    }

    // TODO or return const ref?
    // TODO check if rules_table.value takes in an int
    Rule<Symbol> rule_value(int index) const
    {
        return rules_table.value(index);
    }

    bool is_variable(Symbol sym) const
    {
        auto [lb, ub] = rules_for(sym);
        return lb != ub;
    }

private:
    std::map<Symbol, std::set<Symbol>> first_sets;
    std::set<Rule<Symbol>> rules;
    Enumeration<Rule<Symbol>> rules_table;

    std::set<Symbol> first(Symbol sym) const
    {
        if (!symbols.count(sym)) {
            std::cout << "AssertionError: " << sym 
                << " is not a valid symbol" << std::endl;
            //assert(symbols.count(sym));
            throw 0;
        }
        return first_sets.at(sym);
    }

    void compute_first_sets()
    {
        std::vector<Symbol> variables;
        for (const auto& symbol: symbols) {
            auto [start, end] = rules_for(symbol);
            if (start == end) {
                // i.e. sym is a terminal
                first_sets[symbol] = {symbol};
            } else {
                first_sets[symbol]; // init first_set
                variables.push_back(symbol);
            }
        }

        for (bool changed = true; changed; ) {
            changed = false;
            for (const auto& var: variables) {
                auto [start, end] = rules_for(var);
                for (auto it = start; it != end; ++it) {
                    const auto& rule = *it;
                    for (const auto& symbol: first(rule.rhs)) {
                        auto [_, cond] = first_sets[var].insert(symbol);
                        if (cond) {
                            changed = true;
                        }
                    }
                }
            }
        }
    }
};

} // end namespace
