#pragma once
#include <algorithm>
#include <map>
#include <set>
#include <stdexcept>
#include <utility>
#include <vector>

namespace cfg
{

template <class T, class V>
class Symbol {
    enum class Type { Token, Variable };
    T _token;
    V _variable;
    Type type;

public:
    Symbol(const T& t) : type(Type::Token)
    {
        _token = t;
    }

    Symbol(const V& v) : type(Type::Variable)
    {
        _variable = v;
    }

    const V& variable() const { return _variable; }
    const T& token() const { return _token; }

    bool operator<(const Symbol& other) const
    {
        if (type != other.type) {
            return type == Type::Token;
        }
        if (type == Type::Token) {
            return token() < other.token();
        }
        return variable() < other.variable();
    }

    bool operator==(const Symbol& other) const 
    {
        return !(*this < other) && !(other < *this);
    }

    bool is_variable() const { return type == Type::Variable; }
    bool is_token() const { return type == Type::Token; }
};

template <class Token, class Variable>
class Grammar {
    Token _empty;
public:
    using Sym = Symbol<Token, Variable>;
    using Sentence = typename std::vector<Sym>;
    std::set<Variable> variables;
    std::set<Token> tokens;
    std::map<Variable, std::set<Sentence>> rules;
    std::map<Sym,std::set<Sym>>first_sets;

    Grammar(const Token& _empty) : _empty(_empty) 
    {
        add(_empty);
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
            if (!sym.is_token() && !variables.count(sym.variable())) {
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
            if (!first_set.count(_empty)) {
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
