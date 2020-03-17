#pragma once
#include "parser.hpp"
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

struct ParseTree {
    using Symbol = std::string;
    using Tree = std::unique_ptr<ParseTree>;
    std::vector<Tree> children;
    Symbol value;

    ParseTree(Symbol value) : value(value) {}
};

std::ostream& operator<<(
    std::ostream& out,
    std::unique_ptr<ParseTree>& node)
{
    if (node == nullptr) {
        return out << "nil";
    }
    if (!node->children.empty()) {
        out << "(";
    }
    out << node->value;
    for (auto& child: node->children) {
        out << " " << child;
    }
    if (!node->children.empty()) {
        out << ")";
    }
    return out;
}

bool shift_symbol(BaseCallback* cb, const std::pair<std::string, std::string>& lookahead);

bool reduce_rule(BaseCallback* cb, const std::pair<std::string, std::vector<std::string>>& rule);

struct ParseTreeCallback: public BaseCallback {
    using Symbol = std::string;
    using Rule = std::pair<Symbol, std::vector<Symbol>>;
    std::vector<Symbol> symbols;
    std::vector<std::unique_ptr<ParseTree>> state;

    ParseTreeCallback()
    {
        BaseCallback::shift("a", shift_symbol);
        BaseCallback::shift("b", shift_symbol);
        BaseCallback::reduce("S -> A", reduce_rule);
        BaseCallback::reduce("A -> a A b", reduce_rule);
        BaseCallback::reduce("A ->", reduce_rule);
    }

    auto ast(bool ok)
    {
        if (!ok) {
            throw SyntaxError();
        }
        return std::move(state.back());
    }
};

bool shift_symbol(BaseCallback* cb, const std::pair<std::string, std::string>& lookahead)
{
    ParseTreeCallback* self = (ParseTreeCallback*)cb;
    auto a = lookahead.first;
    self->symbols.push_back(a);
    self->state.push_back(std::make_unique<ParseTree>(a));
    return true;
}

bool reduce_rule(BaseCallback* cb, const std::pair<std::string, std::vector<std::string>>& rule)
{
    ParseTreeCallback* self = (ParseTreeCallback*)cb;
    auto node = std::make_unique<ParseTree>(rule.first);
    std::vector<decltype(node)> children;
    for (int i = 0; i < (int)(rule.second.size()); ++i) {
        self->symbols.pop_back();
        children.push_back(std::move(self->state.back()));
        self->state.pop_back();
    }
    while (!children.empty()) {
        node->children.push_back(std::move(children.back()));
        children.pop_back();
    }
    self->symbols.push_back(rule.first);
    self->state.push_back(std::move(node));
    return true;
}
