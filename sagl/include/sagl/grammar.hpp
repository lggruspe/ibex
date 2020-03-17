#pragma once
#include <map>
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

    Symbol start;
    std::multimap<Symbol, Sentence> rules;
    char const * const empty = "empty";

    Grammar(const Symbol& start, const decltype(rules)& rules)
        : start(start)
        , rules(rules)
    {}

    bool is_terminal(const Symbol& symbol) const
    {
        return rules.find(symbol) == rules.end();
    }
};
