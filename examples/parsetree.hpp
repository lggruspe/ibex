#pragma once
#include "parser.hpp"
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace parser {

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

struct ParseContext {
    using Symbol = std::string;
    using Rule = std::pair<Symbol, std::vector<Symbol>>;
    std::vector<Symbol> symbols;
    std::vector<std::unique_ptr<ParseTree>> state;
};

bool shift_symbol(ParseContext& self, const std::pair<std::string, std::string>& lookahead)
{
    auto a = lookahead.first;
    self.symbols.push_back(a);
    self.state.push_back(std::make_unique<ParseTree>(a));
    return true;
}

bool reduce_rule(ParseContext& self, const std::pair<std::string, std::vector<std::string>>& rule)
{
    auto node = std::make_unique<ParseTree>(rule.first);
    std::vector<decltype(node)> children;
    for (int i = 0; i < (int)(rule.second.size()); ++i) {
        self.symbols.pop_back();
        children.push_back(std::move(self.state.back()));
        self.state.pop_back();
    }
    while (!children.empty()) {
        node->children.push_back(std::move(children.back()));
        children.pop_back();
    }
    self.symbols.push_back(rule.first);
    self.state.push_back(std::move(node));
    return true;
}

struct ParseTreeCallback: public BaseCallback<ParseContext> {
    ParseTreeCallback()
    {
        shift("a", shift_symbol);
        shift("b", shift_symbol);
        reduce("S -> A", reduce_rule);
        reduce("A -> a A b", reduce_rule);
        reduce("A ->", reduce_rule);
    }

    auto ast(bool ok)
    {
        if (!ok) {
            throw SyntaxError();
        }
        return std::move(state.state.back());
    }
};

}
