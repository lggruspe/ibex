#pragma once
#include "grammar.h"
#include "item.h"
#include <algorithm>
#include <list>
#include <map>
#include <set>
#include <vector>

namespace lr1
{

template <class Symbol>
struct Collection {
    std::set<Item<Symbol>> items;
    //std::map<Symbol, CollectionSymbol>> transitions;    // TODO only store pointers to neighbors
    using Sentence = typename std::vector<Symbol>;

    void closure(cfg::Grammar<Symbol>& grammar)
    {
        std::list<Item<Symbol>> queue(items.begin(), items.end());
        while (!queue.empty()) {
            auto item = queue.front();
            queue.pop_front();
            Symbol variable = item.symbol_after_dot();
            if (!grammar.is_variable(variable)) {
                continue;
            }

            // expand variable
            auto subs = grammar.productions(variable);
            for (const auto& sub: subs) {
                Sentence temp = Sentence();// lookahead of new item = grammar.first(temp)
                if (!item.after.empty()) {
                    std::copy(item.after.begin() + 1, item.after.end(), 
                            std::back_inserter(temp));
                }
                temp.push_back(item.lookahead);
                auto first_set = grammar.first(temp);
                for (const auto& sym: first_set) {
                    Item<Symbol> new_item = Item<Symbol>(variable, Sentence(), sub, sym);
                    if (items.insert(new_item).second) {
                        queue.push_back(new_item);
                    } 
                }
            }
        }
    }

    bool operator<(const Collection& coll) const
    {
        return items < coll.items;
    }

    bool operator==(const Collection& coll) const
    {
        return items == coll.items;
    }

    std::map<Symbol, Collection<Symbol>> transition(cfg::Grammar<Symbol>&);
};


template <class Symbol>
std::set<Symbol> transition_symbols(const Collection<Symbol>& state)
{
    std::set<Symbol> symbols;
    for (const auto& item: state.items) {
        Symbol after = item.symbol_after_dot();
        if (!after.empty()) {
            symbols.insert(after);
        }
    }
    return symbols;
}

template <class Symbol>
std::map<Symbol, Collection<Symbol>> 
Collection<Symbol>::transition(cfg::Grammar<Symbol>& grammar)
{
    std::map<Symbol, Collection<Symbol>> transitions;
    std::set<Symbol> symbols = transition_symbols(*this);
    for (const Symbol& sym: symbols) {
        for (const auto& item: items) {
            if (item.symbol_after_dot() == sym) {
                transitions[sym].items.insert(item.shifted());
            }
        }
        transitions[sym].closure(grammar);
    }
    return transitions;
}

} // end namespace
