#pragma once
#include <map>
#include <set>
#include <string>
#include <vector>

struct Grammar {
    // symbols (terminals or variables) are just strings
    // "empty" represents the eof
    // assume first lhs in the set of rules is the start symbol
    // rules must be nonempty, rhs must not contain "empty"
    using Symbol = std::string;
    using Sentence = std::vector<Symbol>;
    using Rule = std::pair<Symbol, Sentence>;

    std::multimap<Symbol, Sentence> rules;
    Symbol start;
    char const * const empty = "empty";

    Grammar(const std::vector<Rule>& rules)
        : start(rules.front().first)
    {
        for (const auto& p: rules) {
            this->rules.insert(p);
        }
    }

    bool is_terminal(const Symbol& symbol) const
    {
        return rules.find(symbol) == rules.end();
    }
};
