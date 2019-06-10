#pragma once
#include <algorithm>
#include <map>
#include <set>
#include <stdexcept>
#include <utility>
#include <variant>
#include <vector>

namespace cfg
{

template <class Token, class Variable>
bool is_token(const std::variant<Token, Variable>& symbol)
{
    return !symbol.index();
}

template <class Token, class Variable>
bool is_variable(const std::variant<Token, Variable>& symbol)
{
    return symbol.index();
}

template <class Token, class Variable>
struct Grammar {
    Token empty;
    using Sym = std::variant<Token, Variable>;
    using Sentence = typename std::vector<Sym>;
    std::set<Variable> variables;
    std::set<Token> tokens;
    std::map<Variable, std::set<Sentence>> rules;
    std::map<Sym,std::set<Sym>>first_sets;

    Grammar(const Token& empty) : empty(empty) 
    {
        add(empty);
    }

    void add(const Token& token)
    {
        auto [it, success] = tokens.insert(token);
        if (success) {
            first_sets[token] = {token};
        }
    }

    void add(const Variable& lhs, const Sentence& rhs)
    {
        // check if rhs is a valid sentence
        for (const auto& sym: rhs) {
            if (!is_token(sym) && !variables.count(std::get<Variable>(sym))) {
                throw std::invalid_argument("sentence has invalid symbol");
            }
        }
        variables.insert(lhs);
        first_sets[lhs];            // initialize
        rules[lhs].insert(rhs);

        // update first sets
        bool changed = true;
        while (changed) {
            changed = false;
            for (const auto& var: variables) {
                std::set<Sym> first_set;
                for (const Sentence& sub: rules[var]) {
                    auto temp = first(sub);
                    first_set.insert(temp.begin(), temp.end());
                }
                if (first_set != first_sets[var]) {
                    changed = true;
                    first_sets[var] = first_set;
                }
            }
        }
    }

    std::set<Sym> first(const Sentence& sent) const
    {
        std::set<Sym> res;
        for (const auto& sym: sent) {
            const auto& first_set = first(sym);
            res.insert(first_set.begin(), first_set.end());
            if (!first_set.count(empty)) {
                break;

            }
        }
        return res;
    }

    const std::set<Sym>& first(const Sym& sym) const
    {
        return first_sets.at(sym);
    }
};

} // end namespace
