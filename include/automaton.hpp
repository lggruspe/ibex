#pragma once
#include "enumeration.hpp"
#include "grammar.hpp"
#include <initializer_list>
#include <iostream>
#include <map>
#include <set>
#include <tuple>
#include <utility>
#include <vector>

namespace sagl
{

template <class Symbol>
struct Item {
    using Sentence = std::vector<Symbol>;
    int rule_id;
    Symbol left;
    Sentence right;
    Symbol lookahead;

    Item(
            int rule_id,
            const std::pair<Symbol, Sentence>& rule,
            Symbol lookahead,
            typename Sentence::size_type dot_index=0)
        : rule_id(rule_id)
        , left(rule.first)
        , right(rule.second)
        , lookahead(lookahead)
        , dot_index(dot_index)
    {}

    Item shifted() const
    {
        return Item(rule_id, {left, right}, lookahead, dot_index+1);
    }

    bool reduces() const
    {
        return dot_index == right.size();
    }

    // symbol after the dot
    typename Sentence::const_iterator after() const
    {
        return right.cbegin() + dot_index;
    }

    // has special meaning when end = 0
    // (from start to lastmost position)
    std::pair<typename Sentence::const_iterator, 
        typename Sentence::const_iterator>
    after_slice(typename Sentence::size_type start, 
            typename Sentence::size_type end) const
    {
        // TODO make sure that the iterators are valid
        return std::make_pair(right.cbegin() + dot_index + start,
                end ? right.cbegin() + dot_index + end
                    : right.cend());
    }

    bool operator<(const Item<Symbol>& other) const
    {
        return std::tie(rule_id, dot_index) 
            < std::tie(other.rule_id, other.dot_index);
    }

    template <class T>
    friend std::ostream& operator<<(std::ostream&, const Item<T>&);

private:
    typename Sentence::size_type dot_index;   // of first symbol after dot
};

template <class Symbol>
struct State {
    std::set<Item<Symbol>> items;

    State() {}
    State(std::initializer_list<Item<Symbol>> items, 
            const Grammar<Symbol>& grammar) : items(items)
    {
        closure(grammar);
    }

    void closure(const Grammar<Symbol>& grammar)
    {
        std::vector<Item<Symbol>> queue(items.begin(), items.end());
        while (!queue.empty()) {
            auto item = queue.back();
            queue.pop_back();
            if (!item.reduces()) {
                auto lhs = *(item.after());
                auto [start, end] = grammar.rules_for(lhs);
                for (auto it = start; it != end; ++it) {
                    auto [_start, _end] = item.after_slice(1, 0);
                    std::vector<Symbol> sentence(_start, _end);
                    sentence.push_back(item.lookahead);
                    auto new_lookaheads = grammar.first(sentence);

                    using Sentence = std::vector<Symbol>;
                    std::pair<Symbol, Sentence> rule = *it;
                    int rule_id = grammar.rule_index(rule);
                    for (const auto& new_lookahead: new_lookaheads) {
                        Item<Symbol> new_item(rule_id, rule, new_lookahead);
                        items.insert(new_item);
                    }
                }
            }
        }
    }

    std::map<Symbol, State> transitions(
            const Grammar<Symbol>& grammar) const {
        std::map<Symbol, State> neighbors;
        for (const auto& item: items) {
            if (!item.reduces()) {
                const auto& lhs = *(item.after());
                neighbors[lhs].insert(item.shifted());
            }
        }

        for (auto& [_, neighbor]: neighbors) {
            neighbor.closure(grammar);
        }
        return neighbors;
    }

    bool operator<(const State<Symbol>& other) const
    {
        return items < other.items;
    }

private:
    void insert(const Item<Symbol>& item)
    {
        items.insert(item);
    }
};

template <class Symbol>
struct Automaton {
    std::map<int, std::map<Symbol, int>> transitions;
    int start;

    Automaton(const Grammar<Symbol>& grammar)
    {
        State<Symbol> state({start_item(grammar)}, grammar);
        start = states.insert(state);
        std::vector<int> queue = {start};
        while (!queue.empty()) {
            auto state_id = queue.back();
            queue.pop_back();
            state = states.value(state_id);
            auto neighbors = state.transitions(grammar);
            for (const auto& [symbol, neighbor]: neighbors) {
                bool push = !states.has_value(neighbor);
                int neighbor_id = states.insert(neighbor);
                transitions[state_id][symbol] = neighbor_id;
                transitions[neighbor_id];
                if (push) {
                    queue.push_back(neighbor_id);
                }
            }
        }
    }

    State<Symbol> state_value(int state_id) const
    {
        return states.value(state_id);
    }

    int state_index(const State<Symbol>& state) const
    {
        return states.index(state);
    }

private:
    Enumeration<State<Symbol>> states;
    Item<Symbol> start_item(const Grammar<Symbol>& grammar) const
    {
        using Sentence = std::vector<Symbol>;
        auto [_start, _] = grammar.rules_for(grammar.start);
        std::pair<Symbol, Sentence> rule = {_start->first, _start->second};
        return Item(grammar.rule_index(rule), rule, grammar.empty);
    }
};

template <class Symbol>
std::ostream& operator<<(std::ostream& out, const Item<Symbol>& item)
{
    out << "{" << item.left << " -> ";
    for (auto i = 0; i < item.right.size(); ++i) {
        if (i == item.dot_index) {
            out << ". ";
        }
        out << item.right[i] << " ";
    }
    if (item.dot_index == item.right.size()) {
        out << ".";
    }
    out << ", " << item.lookahead << "}";
    return out;
}

template <class Symbol>
std::ostream& operator<<(std::ostream& out, const State<Symbol>& state)
{
    out << "State:" << std::endl;
    for (const auto& item: state.items) {
        out << item << std::endl;
    }
    out << std::endl;
    return out;
}

} // end namespace
