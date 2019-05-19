#pragma once
#include "enumeration.h"
#include "grammar.h"
#include <algorithm>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include <iostream>//////////

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

    bool is_reduce() const {
        return after.empty();
    }

    int rule(cfg::Grammar& grammar) {
        cfg::Sentence rhs = before;
        std::copy(after.begin(), after.end(), std::back_inserter(rhs));
        return grammar.rule(cfg::Rule(lhs, rhs));
    }
};


void print_state(const std::vector<Item>& state);   ///////
void print_item(const Item& item);  ////////////////////
void print_sentence(const cfg::Sentence& sent);/////////////


void take_closure(cfg::Grammar&, std::vector<Item>&);
std::map<cfg::Symbol, std::vector<Item>> 
transition(cfg::Grammar&, const std::vector<Item>&);

typedef std::pair<std::string, int> Action;
// first can only be shift, reduce, accept, goto or ""

class Parser {
    std::map<int, std::map<cfg::Symbol, Action>> table;
    Enumeration<std::vector<Item>> names;

public:
    std::map<int, std::map<cfg::Symbol, int>> delta;    /// TODO Move to private

    Parser() {}
    Parser(cfg::Grammar& grammar) { construct_parser(grammar); }

    void construct_automaton(cfg::Grammar& grammar)
    {
        // assume the grammar is augmented
        // rebuilds automaton (delta) and names
        delta.clear();
        names.clear();  // acts as history
        std::vector<Item> start = { 
            Item(grammar.start, {""}, 
            *(grammar.productions(grammar.start).begin()), "")
        };
        take_closure(grammar, start);
        std::vector<int> states = {names.insert(start)};

        if (start.size() != 9) {
            print_state(start);
            throw std::logic_error("start state should have 9 items");
        }

        while (!states.empty()) {
            int name = states.back();
            states.pop_back();
            std::vector<Item> state = names.value(name);
            std::map<cfg::Symbol, std::vector<Item>> transitions = transition(grammar, state);

            for (const auto& p: transitions) {
                if (!names.has_value(p.second)) {
                    states.push_back(names.insert(p.second));
                }
                int pname = names.insert(p.second);
                delta[name][p.first] = pname;
            }
        }
    }

    void construct_parser(cfg::Grammar& grammar) {
        construct_automaton(grammar);
        table.clear();
        for (const auto& transitions: delta) {
            int pname = transitions.first;
            for (const auto& pair: transitions.second) {
                const cfg::Symbol& sym = pair.first;
                int qname = pair.second;

                if (grammar.is_terminal(sym)) {
                    table[pname][sym] = Action("shift", qname);
                } else if (grammar.is_variable(sym)) {
                    // compute goto first
                    table[pname][sym] = Action("goto", qname);
                }

                // compute reduce actions
                // TODO detect shift/reduce and reduce/reduce conflicts
                for (const auto& name_pair: names) {
                    // go through each item in the state
                    const auto& items = name_pair.second;
                    for (auto item: items) {
                        if (!item.is_reduce()) {
                            continue;
                        }
                        if (item.lhs == grammar.start && item.lookahead.empty()) {
                            // check if accept
                            table[pname][sym] = Action("accept", qname);
                        } else {
                            // regular reduce
                            table[pname][item.lookahead] = Action("reduce", item.rule(grammar));
                        }
                    }
                }
            }
        }
    }
};

void take_closure(cfg::Grammar& grammar, std::vector<Item>& closure)
{
    /*
    std::cout << "____________________________________" << std::endl;////
    std::cout << "foo1" << std::endl;   ///
    print_state(closure);               ///
    std::cout << "bar1" << std::endl;   ///
    std::cout << std::endl;
    */

    for (const Item& item: closure) {
        if (item.after.empty()) {
            continue;
        }
        auto variable = item.after.front();
        if (!grammar.is_variable(variable)) {
            continue;
        }

        // expand variable
        auto subs = grammar.productions(variable);
        for (const cfg::Sentence& sub: subs) {
            cfg::Sentence temp;
            if (!item.after.empty()) {
                std::copy(item.after.begin() + 1, item.after.end(), 
                        std::back_inserter(temp));
            }
            temp.push_back(item.lookahead);
            auto first_set = grammar.first(temp);
            for (const cfg::Symbol& sym: first_set) {
                Item new_item(variable, {""}, sub, sym);
                closure.push_back(new_item);
            }
        }
    }

    /*
    std::cout << "foo2" << std::endl;   ///
    print_state(closure);               ///
    std::cout << "bar2" << std::endl;   ///
    std::cout << "____________________________________" << std::endl;////
    */
}

std::set<cfg::Symbol> leading_symbols(const std::vector<Item>& state)
{
    std::set<cfg::Symbol> symbols;
    for (const Item& item: state) {
        if (!item.after.empty()) {  // TODO check lookahead?
            symbols.insert(item.after.front());
        }
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
transition(cfg::Grammar& grammar, const std::vector<Item>& state)
{
    std::map<cfg::Symbol, std::vector<Item>> transitions;
    std::set<cfg::Symbol> symbols = leading_symbols(state);
    for (const cfg::Symbol& sym: symbols) {
        for (const Item& item: state) {
            std::vector<Item> new_items = transition(item, sym);
            std::copy(new_items.begin(), new_items.end(), 
                    std::back_inserter(transitions[sym]));
        }
        take_closure(grammar, transitions[sym]);
    }
    return transitions;
}


////
void print_item(const Item& item)
{
    std::cout << "{" << item.lhs << " -> ";
    for (const auto& sym: item.before) {
        std::cout << sym << ' ';
    }
    std::cout << ".";
    for (const auto& sym: item.after) {
        std::cout << sym << ' ';
    }
    std::cout << ", " << item.lookahead << "}" << std::endl;
}

void print_state(const std::vector<Item>& state)
{
    for (const auto& item: state) {
        print_item(item);
    }
}

void print_sentence(const cfg::Sentence& sent)
{
    for (const auto& sym: sent) {
        std::cout << sym << ' ';
    }
    std::cout << std::endl;
}
/////

} // end namespace
