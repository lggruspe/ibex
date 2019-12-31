#pragma once
#include "../../rnd/src/nexpr.hpp"
#include "variable.hpp"
#include <cstdint>
#include <exception>
#include <iterator>
#include <list>
#include <string>
#include <utility>
#include <vector>

namespace ast {

struct SyntaxError {
    char const* what() const { return "syntax error"; }
};

struct NotImplemented : public std::logic_error {
    NotImplemented() : std::logic_error("not implemented") {}
};

struct InvalidReduction : public std::logic_error {
    InvalidReduction() : std::logic_error("invalid reduction") {}
};

template <class Symbol>
struct Callback {
    using Rule = std::pair<Symbol, std::vector<Symbol>>;
    std::vector<NExpr> stack;

    auto accept(bool acc)
    {
        if (!acc) {
            throw SyntaxError();
        }
        return stack.back();
    }

    void shift(const std::pair<Symbol, std::string>& lookahead)
    {
        switch (auto token = lookahead.first) {
        case Symbol(scanner::Token::DOT):
            ZRange a(0, std::numeric_limits<uint32_t>::max());
            stack.push_back(NExpr(a));
            break;
        case Symbol(scanner::Token::SYMBOL):
            throw NotImplemented();
        case Symbol(scanner::Token::INTERVAL):
            throw NotImplemented();
        default:
            stack.push_back(NExpr());
        }
    }

    NExpr reduce_expr(std::list<NExpr>& rhs) const
    {
        if (auto size = rhs.size(); size == 1) {
            return rhs.front();
        } else if (size == 3) {
            return alternate(rhs.front(), rhs.back());
        } else {
            throw InvalidReduction();
        }
    }

    NExpr reduce_term(std::list<NExpr>& rhs) const
    {
        if (auto size = rhs.size(); size == 1) {
            return rhs.front();
        } else if (size == 2) {
            return concatenate(rhs.front(), rhs.back());
        } else {
            throw InvalidReduction();
        }
    }

    NExpr reduce_factor(std::list<NExpr>& rhs) const
    {
        if (auto size = rhs.size(); size == 1) {
            return rhs.front();
        } else if (size == 2) {
            throw NotImplemented();
        } else {
            throw InvalidReduction();
        }
    }

    NExpr reduce_value(std::list<NExpr>& rhs) const
    {
        if (auto size = rhs.size(); size == 1) {
            return rhs.front();
        } else if (size == 3) {
            // TODO check if front and back are ( )
            return *(std::next(rhs.begin()));
        } else {
            throw InvalidReduction();
        }
    }

    void reduce(const Rule& rule)
    {
        std::list<NExpr> rhs;
        for (int i = 0; i < (int)(rule.second.size()); ++i) {
            rhs.push_front(stack.back());
            stack.pop_back();
        }
        switch (auto lhs = rule.first) {
        case Symbol(Variable::EXPR):
            return stack.push_back(reduce_expr(rhs));
        case Symbol(Variable::TERM):
            return stack.push_back(reduce_term(rhs));
        case Symbol(Variable::FACTOR):
            return stack.push_back(reduce_factor(rhs));
        case Symbol(Variable::VALUE):
            return stack.push_back(reduce_value(rhs));
        }
        throw NotImplemented();
    }
};

}
