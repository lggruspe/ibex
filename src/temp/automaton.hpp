#pragma once
#include "handles.hpp"
#include "grammar.hpp"
#include "items.hpp"
#include <map>
#include <set>
#include <vector>

template <class Grammar>
struct Automaton {
    using Symbol = typename Grammar::Symbol;
    using Sentence = std::vector<Symbol>;
    using State = std::set<Item<Symbol>>;
    State start;
    const Grammar& grammar;
    std::map<int, std::map<Symbol, int>> transitions;

    Automaton(const Grammar& grammar) : grammar(grammar)
    {
        compute_first_sets();
        auto [lb, ub] = grammar.rules.equal_range(grammar.start);
        for (auto it = lb; it != ub; ++it) {
            start.insert(Item<Symbol>(*it, grammar.empty));
        }
        closure(start);
        compute_transitions();
    }

    auto begin() const
    {
        return states.begin();
    }

    auto end() const
    {
        return states.end();
    }

private:
    HandleSet<State> states;
    std::map<Symbol, std::set<Symbol>> first_sets;

    void closure(State& state)
    {
        bool changed = true;
        while (changed) {
            changed = false;
            for (const auto& item: state) {
                if (item.is_complete()) {
                    continue;
                }
                Symbol a = item.next();
                auto [lb, ub] = grammar.rules.equal_range(a);
                for (auto it = lb; it != ub; ++it) {
                    Sentence sentence = item.slice(item.position+1);
                    sentence.push_back(item.lookahead);
                    for (const auto& lookahead: first(sentence)) {
                        auto [_, ok] = state.insert(Item<Symbol>(*it, lookahead));
                        changed = ok || changed;
                    }
                }
            }
        }
    }

    bool compute_transitions (const State& state, int id)
    {
        // assume state has been 'closed'
        std::map<Symbol, State> neighbors;
        for (const auto& item: state) {
            if (!item.is_complete()) {
                Symbol a = item.next();
                neighbors[a].insert(item.shifted());
            }
        }
        bool changed = false;
        for (auto& [a, neighbor]: neighbors) {
            closure(neighbor);
            int r = states.index(neighbor);
            auto [_, ok] = transitions[id].insert(std::make_pair(a, r));
            if (ok) {
                changed = true;
            }
        }
        return changed;
    }
    void compute_transitions()
    {
        int id = states.index(start);
        compute_transitions(start, id);
        bool changed = true;
        while (changed) {
            changed = false;
            for (const auto& [Q, q]: states) {
                bool ok = compute_transitions(Q, q);
                changed = ok || changed;
            }
        }
    }

    void compute_first_sets()
    {
        for (const auto& a: grammar.symbols) {
            first_sets[a];
            if (grammar.is_terminal(a)) {
                first_sets[a].insert(a);
            }
        }
        bool changed = true;
        while (changed) {
            changed = false;
            for (const auto& [lhs, rhs]: grammar.rules) {
                for (const auto& a: first(rhs)) {
                    auto [_, ok] = first_sets[lhs].insert(a);
                    changed = ok || changed;
                }
            }
        }
    }

    std::set<Symbol> first(const Sentence& sentence) const
    {

        std::set<Symbol> first_set;
        for (const auto& a: sentence) {
            auto& f = first_sets.at(a);
            first_set.insert(f.begin(), f.end());
            if (!f.count(grammar.empty)) {
                break;
            }
        }
        return first_set;
    }
};
