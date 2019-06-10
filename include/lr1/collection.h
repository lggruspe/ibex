#pragma once
#include "grammar.h"
#include <algorithm>
#include <list>
#include <map>
#include <set>
#include <utility>
#include <variant>
#include <vector>
#include <tuple>

namespace lr1
{

template <class Token, class Variable>
struct Item {
    using Sym = std::variant<Token, Variable>;
    using Sentence = typename std::vector<Sym>;

    Variable lhs;
    Sentence before;
    Sentence after;
    Sym lookahead;

    Item(const Variable& lhs, const Sentence& before,
            const Sentence& after, const Sym& lookahead) : lhs(lhs), before(before), after(after),
        lookahead(lookahead) {}

    bool operator<(const Item<Token, Variable>& other) const 
    {
        return std::tie(lhs, before, after, lookahead) 
            < std::tie(other.lhs, other.before, other.after, 
                    other.lookahead);
    }

    bool operator==(const Item<Token, Variable>& other) const
    {
        return !(*this < other || other < *this);
    }

    Item<Token, Variable> shifted() const 
    {
        // creates copy with shifted dot
        // after must be nonempty
        if (after.empty()) {
            throw std::logic_error("can't shift before empty string");
        }

        Sentence before = this->before;
        before.push_back(this->after.front());
        Sentence after(this->after.begin() + 1, this->after.end());
        return {lhs, before, after, lookahead};
    }

    bool is_reduce() const 
    {
        return after.empty();
    }
};

template <class Token, class Variable>
std::variant<Token, Variable> symbol_after_dot(const Item<Token, Variable>& item,
        const std::variant<Token, Variable>& if_empty)
{
    return item.after.empty() ? if_empty : item.after.front();
}

template <class Token, class Variable>
struct Collection {
    using Sym = std::variant<Token, Variable>;
    std::set<Item<Token, Variable>> items;
    //std::map<Sym, CollectionSym>> transitions;    // TODO only store pointers to neighbors
    using Sentence = typename std::vector<Sym>;

    void closure(cfg::Grammar<Token, Variable>& grammar)
    {
        std::list<Item<Token, Variable>> queue(items.begin(), items.end());
        while (!queue.empty()) {
            auto item = queue.front();
            queue.pop_front();
            Sym sym = symbol_after_dot(item, Sym(grammar.empty));
            if (!cfg::is_variable(sym)) {
                continue;
            }

            // expand variable (sym)
            for (const auto& sub: grammar.rules[std::get<Variable>(sym)]) {
                Sentence temp = Sentence();// lookahead of new item = grammar.first(temp)
                if (!item.after.empty()) {
                    std::copy(item.after.begin() + 1, item.after.end(), 
                            std::back_inserter(temp));
                }
                temp.push_back(item.lookahead);
                auto first_set = grammar.first(temp);
                for (const auto& first: first_set) {
                    Item<Token, Variable> new_item(std::get<Variable>(sym), Sentence(), sub, first);
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

    std::set<Sym> _transition_symbols()
    {
        std::set<Sym> symbols;
        for (const auto& item: items) {
            if (!item.after.empty()) {
                symbols.insert(item.after.front());
            }
        }
        return symbols;
    }

    std::map<Sym, Collection<Token, Variable>> transition(
            cfg::Grammar<Token, Variable>& grammar)
    {
        std::map<Sym, Collection<Token, Variable>> transitions;
        std::set<Sym> symbols = _transition_symbols();
        for (const auto& sym: symbols) {
            for (const auto& item: items) {
                if (symbol_after_dot(item, Sym(grammar.empty)) == sym) {
                    transitions[sym].items.insert(item.shifted());
                }
            }
            transitions[sym].closure(grammar);
        }
        return transitions;
    }
};

} // end namespace
