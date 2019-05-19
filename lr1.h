#pragma once
#include "enumeration.h"
#include "grammar.h"
#include <algorithm>
#include <map>
#include <set>
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

    bool operator<(const Item& rhs) const {
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

    bool operator==(const lr1::Item& rhs) const {
        return !(*this < rhs) && !(rhs < *this);
    }

    bool operator!=(const lr1::Item& rhs) const {
        return !(*this == rhs);
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

    bool can_expand(const cfg::Grammar& grammar) const {
        if (after.empty()) {
            return false;
        }
        cfg::Symbol sym = after.front();
        return grammar.is_variable(sym);
    }
};

typedef std::set<Item> Collection;

void print_state(const Collection& state);   ///////
void print_item(const Item& item);  ////////////////////
void print_sentence(const cfg::Sentence& sent);/////////////


void take_closure(cfg::Grammar&, Collection&);
std::map<cfg::Symbol, Collection> 
transition(cfg::Grammar&, const Collection&);

typedef std::pair<std::string, int> Action;
// first can only be shift, reduce, accept, goto or ""

class Parser {
public:
    std::map<int, std::map<cfg::Symbol, Action>> table; // TODO make priv.
    Enumeration<Collection> names;// TODO move to private
    std::map<int, std::map<cfg::Symbol, int>> delta;    /// TODO Move to private

    Parser() {}
    Parser(cfg::Grammar& grammar) { construct_parser(grammar); }

    void construct_automaton(cfg::Grammar& grammar)
    {
        // assume the grammar is augmented
        // rebuilds automaton (delta) and names
        delta.clear();
        names.clear();  // acts as history
        Collection start = { 
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
            Collection state = names.value(name);
            std::map<cfg::Symbol, Collection> transitions = transition(grammar, state);

            for (const auto& p: transitions) {
                int pname;
                if (!names.has_value(p.second)) {
                    pname = names.insert(p.second);
                    states.push_back(pname);
                } else {
                    pname = names.index(p.second);
                }
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

void take_closure(cfg::Grammar& grammar, Collection& closure)
{
    for (const Item& item: closure) {
        if (!item.can_expand(grammar)) {
            continue;
        }
        cfg::Symbol variable = item.after.front();

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
                closure.insert(new_item);
            }
        }
    }
}

std::set<cfg::Symbol> leading_symbols(const Collection& state)
{
    std::set<cfg::Symbol> symbols;
    for (const Item& item: state) {
        if (!item.after.empty()) {  // TODO check lookahead?
            symbols.insert(item.after.front());
        }
    }
    return symbols;
}

Collection transition(const Item& item, const cfg::Symbol& sym)
{
    Collection res;
    if (!item.after.empty() && item.after.front() == sym) {
        res.insert(item.shift());
    }
    return res;
}

std::map<cfg::Symbol, Collection> 
transition(cfg::Grammar& grammar, const Collection& state)
{
    std::map<cfg::Symbol, Collection> transitions;
    std::set<cfg::Symbol> symbols = leading_symbols(state);
    for (const cfg::Symbol& sym: symbols) {
        for (const Item& item: state) {
            Collection new_items = transition(item, sym);
            transitions[sym].insert(new_items.begin(), new_items.end());
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

void print_state(const Collection& state)
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

void print_collections(const Parser& parser)
{
    for (const auto& state: parser.names) {
        std::cout << state.first << std::endl;
        print_state(state.second);
        std::cout << std::endl;
    }

}
////////////
/////

} // end namespace
