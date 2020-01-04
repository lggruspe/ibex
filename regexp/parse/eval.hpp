#pragma once
#include "variable.hpp"
#include "../scan/scanner.hpp"
#include "../../rnd/src/automaton.hpp"
#include "../../rnd/src/nexpr.hpp"
#include "../../rnd/src/zsymbols.hpp"
#include <cstdint>
#include <exception>
#include <limits>
#include <set>
#include <string>
#include <utility>
#include <vector>

namespace eval {

uint32_t to_symbol(const std::string& lexeme)
{
    if (auto size = lexeme.size(); size == 1) {
        return lexeme[0];
    } else if (size >= 2 && lexeme[0] == '\\') {
        if (auto a = lexeme[1]; a != 'x' && a != 'X') {
            return a;
        }
        uint32_t x = 0;
        for (int i = 2; i < (int)(lexeme.size()); ++i) {
            auto a = lexeme[i];
            x *= 16;
            if ('0' <= a && a <= '9') {
                x += (a - '0');
            } else if ('a' <= a && a <= 'f') {
                x += (a - 'a' + 10);
            } else if ('A' <= a && a <= 'F') {
                x += (a - 'A' + 10);
            } else {
                throw std::logic_error("invalid shift");
            }
        }
        // TODO raise error if x overflows
        return x;
    } else {
        throw std::logic_error("invalid shift");
    }
}

template <class Symbol>
struct Callback {
    std::vector<rnd::NExpr> state;

    auto accept(bool acc)
    {
        if (!acc) {
            throw std::logic_error("syntax error");
        }
        return rnd::Automaton(state.back());
    }

    void shift(const std::pair<Symbol, std::string>& lookahead)
    {
        auto [token, lexeme] = lookahead;
        std::set<Symbol> nulls{scanner::Token::PIPE, scanner::Token::STAR,
            scanner::Token::LPAREN, scanner::Token::RPAREN,
            scanner::Token::QUESTION, scanner::Token::PLUS,
            scanner::Token::LBRACKET, scanner::Token::RBRACKET,
            scanner::Token::DASH,
        };
        if (nulls.count(token)) {
            state.push_back(rnd::NExpr(rnd::ZRange(lexeme[0], lexeme[0]+1)));
        } else if (token == Symbol(scanner::Token::DOT)) {
            auto limit = std::numeric_limits<uint32_t>::max();
            state.push_back(rnd::NExpr(rnd::ZRange(0, limit)));
        } else if (token == Symbol(scanner::Token::SYMBOL)) {
            auto a = to_symbol(lexeme);
            state.push_back(rnd::NExpr(rnd::ZRange(a, a+1)));
        } else {
            throw std::logic_error("invalid shift");
        }
    }

    void reduce_expr(std::vector<rnd::NExpr>& rhs)
    {
        if (auto size = rhs.size(); size == 1) {
            state.push_back(rhs[0]);
        } else if (size == 3) {
            state.push_back(rnd::alternate(rhs[0], rhs[2]));
        } else {
            throw std::logic_error("invalid reduce");
        }
    }

    void reduce_term(std::vector<rnd::NExpr>& rhs)
    {
        if (auto size = rhs.size(); size == 1) {
            state.push_back(rhs[0]);
        } else if (size == 2) {
            state.push_back(rnd::concatenate(rhs[0], rhs[1]));
        } else {
            throw std::logic_error("invalid reduce");
        }
    }

    void reduce_factor(std::vector<rnd::NExpr>& rhs)
    {
        if (auto size = rhs.size(); size == 1) {
            state.push_back(rhs[0]);
        } else if (size == 2) {
            auto a = rhs[1].states.at(0).begin()->first;
            switch (a.start) {
            case '*':
                state.push_back(rnd::closure(rhs[0]));
                break;
            case '?':
                state.push_back(rnd::alternate(rhs[0], rnd::NExpr()));
                break;
            case '+':
                state.push_back(rnd::concatenate(rhs[0], rnd::closure(rhs[0])));
                break;
            default:
                throw std::logic_error("invalid reduce");
            }
        } else {
            throw std::logic_error("invalid reduce");
        }
    }

    void reduce_value(std::vector<rnd::NExpr>& rhs)
    {
        if (auto size = rhs.size(); size == 1) {
            state.push_back(rhs[0]);
        } else if (size == 3) {
            state.push_back(rhs[1]);
        } else {
            throw std::logic_error("invalid reduce");
        }
    }

    void reduce_simple(std::vector<rnd::NExpr>& rhs)
    {
        state.push_back(rhs[0]);
    }

    void reduce_compound(std::vector<rnd::NExpr>& rhs)
    {
        if (rhs.size() == 3) {
            auto a = rhs[0].states.at(0).begin()->first.start;
            auto b = rhs[2].states.at(0).begin()->first.start;
            if ((a == '(' && b == ')') || (a == '[' && b == ']')) {
                state.push_back(rhs[1]);
            } else {
                throw std::logic_error("invalid reduce");
            }
        } else {
            throw std::logic_error("invalid reduce");
        }
    }

    void reduce_list(std::vector<rnd::NExpr>& rhs)
    {
        if (auto size = rhs.size(); size == 1) {
            state.push_back(rhs[0]);
        } else if (size == 2) {
            state.push_back(rnd::alternate(rhs[0], rhs[1]));
        } else if (size == 3) {
            auto a = rhs[0].states.at(0).begin()->first.start;
            auto b = rhs[2].states.at(0).begin()->first.end;
            state.push_back(rnd::ZRange(a, b));
        } else {
            throw std::logic_error("invalid reduce");
        }
    }

    using Rule = std::pair<Symbol, std::vector<Symbol>>;
    void reduce(const Rule& rule)
    {
        if (rule.second.size() > state.size()) {
            throw std::logic_error("invalid reduce");
        }
        std::vector<rnd::NExpr> rhs(
            state.begin() + (state.size() - rule.second.size()),
            state.end());
        for (int i = 0; i < (int)(rule.second.size()); ++i) {
            state.pop_back();
        }
        auto lhs = rule.first;
        if (lhs == Symbol(Variable::EXPR)) {
            reduce_expr(rhs);
        } else if (lhs == Symbol(Variable::TERM)) {
            reduce_term(rhs);
        } else if (lhs == Symbol(Variable::FACTOR)) {
            reduce_factor(rhs);
        } else if (lhs == Symbol(Variable::VALUE)) {
            reduce_value(rhs);
        } else if (lhs == Symbol(Variable::SIMPLE)) {
            reduce_simple(rhs);
        } else if (lhs == Symbol(Variable::COMPOUND)) {
            reduce_compound(rhs);
        } else if (lhs == Symbol(Variable::LIST)) {
            reduce_list(rhs);
        } else {
            throw std::logic_error("invalid reduce");
        }
    }
};

}
