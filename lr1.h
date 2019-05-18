#pragma once
#include "bimap.h"
#include "grammar.h"
#include <algorithm>
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace lr1
{

class Item {
public:
    cfg::Symbol lhs;
    cfg::Sentence before;
    cfg::Sentence after;
    cfg::Symbol lookahead;

    Item() {}
    Item(const cfg::Symbol& lhs, const cfg::Sentence& before,
            const cfg::Sentence& after, const cfg::Symbol& lookahead)
    {
        this->lhs = lhs;
        this->before = before;
        this->after = after;
        this->lookahead = lookahead;
    }

    bool operator<(const lr1::Item& rhs) const {
        if (lhs < rhs.lhs) {
            return true;
        }
        if (before < rhs.before) {
            return true;
        }
        if (after < rhs.after) {
            return true;
        }
        return lookahead < rhs.lookahead;
    }

    Item shift() const {
        // creates copy with shifted dot
        // assume after is nonempty
        cfg::Sentence before = this->before;
        before.push_back(this->after.front());
        cfg::Sentence after(this->after.begin() + 1, this->after.end());
        Item res(lhs, before, after, lookahead);
        return res;
    }
};

void take_closure(const cfg::Grammar&, std::vector<Item>&);
std::map<cfg::Symbol, std::vector<Item>> 
transition(const cfg::Grammar&, const std::vector<Item>&);

typedef std::pair<std::string, int> Action;
// first can only be shift, reduce, accept, goto or ""

class Parser {
    std::map<int, std::map<cfg::Symbol, Action>> table;

public:
    Parser() {}
    Parser(const cfg::Grammar& grammar) { construct(grammar); } 

    void construct(const cfg::Grammar& grammar)
    {
        // assume the grammar is augmented
        std::vector<Item> start = { 
            Item(grammar.start, {""}, 
            *(grammar.get_substitutes(grammar.start).begin()), "")
        };
        take_closure(grammar, start);
        Bimap<int, std::vector<Item>> names;        // acts as history
        names.insert(start);
        std::vector<int> states = {names.rget(start)};

        while (!states.empty()) {
            int name = states.back();
            states.pop_back();
            std::vector<Item> state = names.get(name);

            std::map<cfg::Symbol, std::vector<Item>> delta = transition(grammar, state);
            for (const auto& p: delta) {
                if (!names.rcontains(p.second)) {
                    states.push_back(names.insert(p.second));
                }
                int pname = names.rget(p.second);
                table[name][p.first] = std::pair<std::string,int>("goto", pname);
            }
            // TODO Continue here
        }
    }
};

void take_closure(const cfg::Grammar& grammar, std::vector<Item>& closure)
{
    for (const Item& item: closure) {
        auto variable = item.after.front();
        if (!grammar.is_variable(variable)) {
            continue;
        }
        // expand variable
        auto subs = grammar.get_substitutes(variable);
        for (const cfg::Sentence& sub: subs) {
            cfg::Sentence temp(item.after.begin() + 1, item.after.end());
            temp.push_back(item.lookahead);
            auto first_set = grammar.first(temp);
            for (const cfg::Symbol& sym: first_set) {
                Item new_item(variable, {""}, sub, sym);
                closure.push_back(new_item);
            }
        }
    }
}

std::set<cfg::Symbol> leading_symbols(const std::vector<Item>& state)
{
    std::set<cfg::Symbol> symbols;
    for (const Item& item: state) {
        symbols.insert(item.after.front());
    }
    return symbols;
}

std::vector<Item> transition(const Item& item, const cfg::Symbol& sym)
{
    std::vector<Item> res;
    if (!item.after.empty() && item.after.front() == sym) {
        res.push_back(item.shift());
    }
    return res;
}

std::map<cfg::Symbol, std::vector<Item>> 
transition(const cfg::Grammar& grammar, const std::vector<Item>& state)
{
    std::map<cfg::Symbol, std::vector<Item>> delta;
    std::set<cfg::Symbol> symbols = leading_symbols(state);
    for (const cfg::Symbol& sym: symbols) {
        for (const Item& item: state) {
            std::vector<Item> new_items = transition(item, sym);
            std::copy(new_items.begin(), new_items.end(), 
                    std::back_inserter(delta[sym]));
        }
        take_closure(grammar, delta[sym]);
    }
    return delta;
}

} // end namespace
