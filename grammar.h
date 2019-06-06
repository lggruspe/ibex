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
struct Symbol {
    enum class Type { Token, Variable };
    union _Symbol {
        T t;
        V v;
    };
    Type type;
    _Symbol _symbol;

    Symbol(const T& t) : type(Type::Token)
    {
        _symbol.t = t;
    }

    Symbol(const V& v) : type(Type::Variable)
    {
        _symbol.v = v;
    }

    V variable() const { return _symbol.v; }
    T token() const { return _symbol.t; }

    bool operator<(const Symbol& other) const
    {
        if (type != other.type) {
            return type == Type::Token;
        }
        if (type == Type::Token) {
            return _symbol.t < other._symbol.t;
        }
        return _symbol.v < other._symbol.v;
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
            for (const auto& sym: variables) {
                std::set<Sym> first_set;
                for (const Sentence& sub: rules[sym]) {
                    if (sub[0].is_token()) {
                        first_set.insert(sub[0]);
                    } else {
                        auto temp = first(sub);
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
