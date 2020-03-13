#pragma once
#include <set>
#include <map>
#include <vector>

struct Grammar {
    // symbols are either variables or terminals
    // terminals are represented by string literals
    // "empty" represents the eof
    // assume first lhs in the set of rules is the start symbol
    // rules must be nonempty, rhs must not contain "empty"
    using Symbol = char const *;
    using Sentence = std::vector<Symbol>;
    using Rule = std::pair<Symbol, Sentence>;

    std::set<Symbol> symbols;
    std::multimap<Symbol, Sentence> rules;
    Symbol start;
    char const * const empty = "empty";

    Grammar(const std::vector<Rule>& rules)
        : start(rules.front().first)
    {
        symbols.insert(empty);
        for (const auto& p: rules) {
            this->rules.insert(p);
            symbols.insert(p.first);
            for (const auto& a: p.second) {
                if (start == a) {
                    throw 0;
                }
                symbols.insert(a);
            }
        }
    }

    bool is_terminal(const Symbol& symbol) const
    {
        return rules.find(symbol) == rules.end();
    }
};
