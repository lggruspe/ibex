#pragma once
#include "enumeration.hpp"
#include "rule.hpp"
#include <map>
#include <set>
#include <utility>
#include <vector>

namespace sagl
{

template <class Symbol>
struct Grammar {
    using Sentence = std::vector<Symbol>;

    std::set<Symbol> symbols;
    Symbol start;
    Symbol empty;
   
    Grammar(Symbol start, Symbol empty, 
            const std::multimap<Symbol, Sentence>& rules)
        : start(start)
        , empty(empty)
        , rules(rules)
    {
        initialize_symbols();
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

    template <class T = typename std::multimap<Symbol, Sentence>::const_iterator>
    std::pair<T, T> rules_for(Symbol sym) const
    {
        return { rules.lower_bound(sym), rules.upper_bound(sym) };
    }

    int rule_index(const Rule<Symbol>& rule) const
    {
        return rules_table.index(rule);
    }

    const Rule<Symbol>& rule_value(int index) const
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
    std::multimap<Symbol, Sentence> rules;
    Enumeration<Rule<Symbol>> rules_table;

    std::set<Symbol> first(Symbol sym) const
    {
        return first_sets.at(sym);
    }

    void compute_first_sets()
    {
        std::vector<Symbol> variables;
        for (const auto& symbol: symbols) {
            if (is_variable(symbol)) {
                first_sets[symbol]; // init first_set
                variables.push_back(symbol);
            } else {
                first_sets[symbol] = {symbol};
            }
        }

        for (bool changed = true; changed; ) {
            changed = false;
            for (const auto& var: variables) {
                auto [start, end] = rules_for(var);
                for (auto it = start; it != end; ++it) {
                    const auto& [lhs, sentence] = *it;
                    Rule rule(lhs, sentence);
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

    void initialize_symbols()
    {
        symbols.insert(start);
        symbols.insert(empty);
        for (const auto& [symbol, sentence]: rules) {
            rules_table.insert(Rule(symbol, sentence));
            symbols.insert(symbol);
            symbols.insert(sentence.begin(), sentence.end());;
        }
    }
};

} // end namespace
