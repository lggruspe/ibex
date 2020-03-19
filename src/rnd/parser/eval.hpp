#pragma once
#include "scanner.hpp"
#include "parser.hpp"
#include "rnd/internals/automaton.hpp"
#include "rnd/internals/nexpr.hpp"
#include "rnd/internals/zsymbols.hpp"
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

struct Context {
    using Symbol = std::string;
    using Rule = std::pair<Symbol, std::vector<Symbol>>;

    std::vector<rnd::NExpr> stack;
   
    std::vector<rnd::NExpr> get_rhs(const Rule& rule)
    {
        if (rule.second.size() > stack.size()) {
            throw std::logic_error("invalid reduce");
        }
        std::vector<rnd::NExpr> rhs(
            stack.begin() + (stack.size() - rule.second.size()),
            stack.end());
        for (int i = 0; i < (int)(rule.second.size()); ++i) {
            stack.pop_back();
        }
        return rhs;
    }
};

bool shift_all(Context& self, const std::pair<std::string, std::string>& lookahead)
{
    auto [token, lexeme] = lookahead;
    const static std::set<std::string> nulls{
        "pipe", "star", "lparen", "rparen", "question", "plus", "lbracket",
        "rbracket", "dash",
    };
    if (nulls.count(std::string(token))) {
        self.stack.push_back(rnd::NExpr(rnd::ZRange(lexeme[0], lexeme[0]+1)));
    } else if (std::string(token) == std::string("dot")) {
        auto limit = std::numeric_limits<uint32_t>::max();
        self.stack.push_back(rnd::NExpr(rnd::ZRange(0, limit)));
    } else if (std::string(token) == std::string("symbol")) {
        auto a = to_symbol(lexeme);
        self.stack.push_back(rnd::NExpr(rnd::ZRange(a, a+1)));
    } else {
        return false;
    }
    return true;
}

using Rule = std::pair<std::string, std::vector<std::string>>;

bool reduce_expr(Context& self, const Rule& rule)
{
    auto rhs = self.get_rhs(rule);
    if (auto size = rhs.size(); size == 1) {
        self.stack.push_back(rhs[0]);
    } else if (size == 3) {
        self.stack.push_back(rnd::alternate(rhs[0], rhs[2]));
    } else {
        throw std::logic_error("invalid reduce");
    }
    return true;
}

bool reduce_term(Context& self, const Rule& rule)
{
    auto rhs = self.get_rhs(rule);
    if (auto size = rhs.size(); size == 1) {
        self.stack.push_back(rhs[0]);
    } else if (size == 2) {
        self.stack.push_back(rnd::concatenate(rhs[0], rhs[1]));
    } else {
        throw std::logic_error("invalid reduce");
    }
    return true;
}

bool reduce_factor(Context& self, const Rule& rule)
{
    auto rhs = self.get_rhs(rule);
    if (auto size = rhs.size(); size == 1) {
        self.stack.push_back(rhs[0]);
    } else if (size == 2) {
        auto a = rhs[1].states.at(0).begin()->first;
        switch (a.start) {
        case '*':
            self.stack.push_back(rnd::closure(rhs[0]));
            break;
        case '?': {
            self.stack.push_back(rnd::alternate(rhs[0], rnd::epsilon()));
            break;
        }
        case '+':
            self.stack.push_back(rnd::concatenate(rhs[0], rnd::closure(rhs[0])));
            break;
        default:
            throw std::logic_error("invalid reduce");
        }
    } else {
        throw std::logic_error("invalid reduce");
    }
    return true;
}

bool reduce_value(Context& self, const Rule& rule)
{
    auto rhs = self.get_rhs(rule);
    if (auto size = rhs.size(); size == 1) {
        self.stack.push_back(rhs[0]);
    } else if (size == 3) {
        self.stack.push_back(rhs[1]);
    } else {
        throw std::logic_error("invalid reduce");
    }
    return true;
}

bool reduce_simple(Context& self, const Rule& rule)
{
    return true;
}

bool reduce_compound(Context& self, const Rule& rule)
{
    auto rhs = self.get_rhs(rule);
    if (rhs.size() == 3) {
        auto a = rhs[0].states.at(0).begin()->first.start;
        auto b = rhs[2].states.at(0).begin()->first.start;
        if ((a == '(' && b == ')') || (a == '[' && b == ']')) {
            self.stack.push_back(rhs[1]);
        } else {
            throw std::logic_error("invalid reduce");
        }
    } else {
        throw std::logic_error("invalid reduce");
    }
    return true;
}

bool reduce_list(Context& self, const Rule& rule)
{
    auto rhs = self.get_rhs(rule);
    if (auto size = rhs.size(); size == 1) {
        self.stack.push_back(rhs[0]);
    } else if (size == 2) {
        self.stack.push_back(rnd::alternate(rhs[0], rhs[1]));
    } else {
        throw std::logic_error("invalid reduce");
    }
    return true;
}

bool reduce_element(Context& self, const Rule& rule)
{
    auto rhs = self.get_rhs(rule);
    if (auto size = rhs.size(); size == 1) {
        self.stack.push_back(rhs[0]);
    } else if (size == 3) {
        auto a = rhs[0].states.at(0).begin()->first.start;
        auto b = rhs[2].states.at(0).begin()->first.end;
        self.stack.push_back(rnd::ZRange(a, b));
    } else {
        throw std::logic_error("invalid reduce");
    }
    return true;
}

struct Callback: public parser::BaseCallback<Context> {
    Callback()
    {
        reduce("expr -> expr pipe term", reduce_expr);
        reduce("expr -> term", reduce_expr);
        reduce("term -> term factor", reduce_term);
        reduce("term -> factor", reduce_term);
        reduce("factor -> value star", reduce_factor);
        reduce("factor -> value plus", reduce_factor);
        reduce("factor -> value question", reduce_factor);
        reduce("factor -> value", reduce_factor);
        reduce("value -> simple", reduce_value);
        reduce("value -> compound", reduce_value);
        reduce("simple -> dot", reduce_simple);
        reduce("simple -> symbol", reduce_simple);
        reduce("compound -> lparen expr rparen", reduce_compound);
        reduce("compound -> lbracket list rbracket", reduce_compound);
        reduce("list -> list element", reduce_list);
        reduce("list -> element", reduce_list);
        reduce("element -> symbol", reduce_element);
        reduce("element -> symbol dash symbol", reduce_element);
    }

    auto accept(bool acc)
    {
        if (!acc) {
            throw std::logic_error("syntax error");
        }
        return rnd::Automaton(state.stack.back());
    }

    bool handle_shift(const std::pair<std::string, std::string>& lookahead)
    {
        if (bool ok = shift_all(state, lookahead); !ok) {
            throw std::logic_error("invalid shift");
        }
        return true;
    }
};

}
