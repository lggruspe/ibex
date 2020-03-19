#include "tabulate.hpp"
#include "grammar.hpp"
#include "utils/handles.hpp"
#include <cassert>
#include <map>
#include <set>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

struct Item {
    using Symbol = std::string;
    using Sentence = std::vector<Symbol>;
    using Rule = std::pair<Symbol, Sentence>;

    Rule rule;
    Symbol lookahead;
    int position;

    bool is_complete() const
    {
        return position == (int)(rule.second.size());
    }

    Symbol next() const
    {
        // must not be complete
        // returns symbol after the dot
        return rule.second[position];
    }

    Sentence slice(int start = -1, int end = -1) const
    {
        start = (start == -1 ? position : start);
        end = (end == -1 ? rule.second.size() : end);
        return Sentence(rule.second.begin() + start, rule.second.begin() + end);
    }

    Item(const Rule& rule, Symbol lookahead)
        : rule(rule)
        , lookahead(lookahead)
        , position(0)
    {}

    Item shifted() const
    {
        if (is_complete()) {
            throw 0;
        }
        Item item = *this;
        item.position++;
        return item;
    }

    bool operator<(const Item& other) const
    {
        return std::tie(rule, lookahead, position) <
            std::tie(other.rule, other.lookahead, other.position);
    }
};

struct Automaton {
    using Symbol = typename Grammar::Symbol;
    using Sentence = std::vector<Symbol>;
    using State = std::set<Item>;
    State start;
    const Grammar& grammar;
    std::set<Symbol> grammar_symbols;
    std::map<int, std::map<Symbol, int>> transitions;

    Automaton(const Grammar& grammar) : grammar(grammar)
    {
        compute_grammar_symbols();
        compute_first_sets();
        auto [lb, ub] = grammar.rules.equal_range(grammar.start);
        for (auto it = lb; it != ub; ++it) {
            start.insert(Item(*it, grammar.empty));
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
                        auto [_, ok] = state.insert(Item(*it, lookahead));
                        changed = ok || changed;
                    }
                }
            }
        }
    }

    bool compute_transitions(const State& state, int id)
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

    void compute_grammar_symbols()
    {
        grammar_symbols.insert(grammar.empty);
        for (const auto& p: grammar.rules) {
            grammar_symbols.insert(p.first);
            for (const auto& a: p.second) {
                assert(a != grammar.start);
                grammar_symbols.insert(a);
            }
        }
    }

    void compute_first_sets()
    {
        for (const auto& a: grammar_symbols) {
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
            const auto& f = first_sets.at(a);
            first_set.insert(f.begin(), f.end());
            if (!f.count(grammar.empty)) {
                break;
            }
        }
        return first_set;
    }
};

void Table::set(int q, Symbol c, Action a, int v = 0)
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

Table::Table(const Grammar& grammar) : grammar_ptr(&grammar)
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
