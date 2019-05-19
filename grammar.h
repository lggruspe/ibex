#pragma once
#include "enumeration.h"
#include <algorithm>
#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace cfg
{

typedef std::string Symbol;
typedef std::vector<Symbol> Sentence;
typedef std::pair<Symbol, Sentence> Rule;

class Grammar {
    std::set<Symbol> variables;
    std::set<Symbol> terminals;
    Enumeration<Rule> rules;
    std::map<Symbol,std::set<Symbol>> first_sets;
    bool first_sets_valid;

    std::set<Symbol> unchecked_first(const Sentence& sent) const;
public:
    Symbol start;
    Grammar(): first_sets_valid(false){}

    void add_terminal(const Symbol& sym);
    void add_variable(const Symbol& sym);
    void add_rule(const Rule& rule);
    void add_rule(const Symbol& lhs, const Sentence& rhs);
    void set_start(const Symbol& sym);

    bool is_variable(const Symbol& sym) const;
    bool is_terminal(const Symbol& sym) const;
    std::set<Sentence> get_substitutes(const Symbol& sym) const;

    std::set<Symbol> first(const Sentence& sent) const;
    std::set<Symbol> first(const Symbol& sym) const;

    void compute_first_sets();      // TODO call this in first
                                    // if first_sets_valid is false
};

void Grammar::add_terminal(const Symbol& sym)
{
    first_sets_valid = false;
    terminals.insert(sym);
}

void Grammar::add_variable(const Symbol& sym)
{
    first_sets_valid = false;
    variables.insert(sym);
}

void Grammar::add_rule(const Rule& rule)
{
    // TODO check if lhs is a variable
    first_sets_valid = false;
    rules.insert(rule);
}

void Grammar::add_rule(const Symbol& lhs, const Sentence& rhs)
{
    add_rule(Rule(lhs, rhs));
}

void Grammar::set_start(const Symbol& sym)
{
    if (!is_variable(sym)) {
        throw std::invalid_argument("input symbol not a grammar variable");
    }
    start = sym;
}

bool Grammar::is_variable(const Symbol& sym) const
{
    return variables.find(sym) != variables.end();
}

bool Grammar::is_terminal(const Symbol& sym) const
{
    return terminals.find(sym) != terminals.end();
}

std::set<Sentence> Grammar::get_substitutes(const Symbol& sym) const
{
    if (!is_variable(sym)) {
        throw std::invalid_argument("input symbol is not a variable");
    }

    std::set<Sentence> res;
    auto it = std::find_if(rules.ibegin(), rules.iend(),
            rule_predicate(sym, true));
    auto jt = std::find_if(it, rules.iend(),
            rule_predicate(sym, false));
    for (; it != jt; ++it) {
        res.insert(it->second);
    }
    return res;
}

std::set<Symbol> Grammar::first(const Symbol& sym) const
{
    if (!first_sets_valid) {
        throw std::logic_error("called first before compute_first_sets");
    }
    std::set<Symbol> res = first_sets.at(sym);
    return res;
}

std::set<Symbol> Grammar::unchecked_first(const Sentence& sent) const
{
    std::set<Symbol> res;
    for (const Symbol& sym: sent) {
        if (first_sets.find(sym) == first_sets.end()) { // because first_sets.at causes an error
            continue;                                   //
        }                                               //
        const auto& first_set = first_sets.at(sym);
        std::copy(first_set.begin(), first_set.end(),
                std::inserter(res, res.begin()));
        if (first_set.find("") == first_set.end()) {
            break;
        }
    }
    return res;
}

std::set<Symbol> Grammar::first(const Sentence& sent) const
{
    if (!first_sets_valid) {
        throw std::logic_error("called first before compute_first_sets");
    }
    return unchecked_first(sent);
}

void Grammar::compute_first_sets()
{
    for (const Symbol& term: terminals) {
        first_sets[term] = {term};
    }

    bool changed = true;
    while (changed) {
        changed = false;
        for (const Symbol& sym: variables) {
            std::set<Sentence> subs = get_substitutes(sym);
            std::set<Symbol> first_set;
            for (const Sentence& sub: subs) {
                if (is_terminal(sub[0])) {
                    first_set.insert(sub[0]);
                } else {
                    std::set<Symbol> temp = unchecked_first(sub);
                    std::copy(temp.begin(), temp.end(),
                            std::inserter(first_set, first_set.begin()));
                }
            }

            if (first_set != first_sets[sym]) {
                changed = true;
                first_sets[sym] = first_set;
            }
        }
    }
    first_sets_valid = true;
}
} // end namespace
