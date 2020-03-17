#pragma once
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

template <class Symbol>
struct ParseTree {
    using Tree = std::unique_ptr<ParseTree<Symbol>>;
    std::vector<Tree> children;
    Symbol value;

    ParseTree(Symbol value) : value(value) {}
};

struct SyntaxError {
    char const* what() const { return "syntax error"; }
};

template <class Symbol>
std::ostream& operator<<(
    std::ostream& out,
    std::unique_ptr<ParseTree<Symbol>>& node)
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

template <class Symbol>
struct ParseTreeCallback {
    using Rule = std::pair<Symbol, std::vector<Symbol>>;
    std::vector<Symbol> symbols;
    std::vector<std::unique_ptr<ParseTree<Symbol>>> state;

    auto accept(bool acc)
    {
        if (!acc) {
            throw SyntaxError();
        }
        return std::move(state.back());
    }

    void shift(const std::pair<Symbol, std::string>& lookahead)
    {
        auto a = lookahead.first;
        symbols.push_back(a);
        state.push_back(std::make_unique<ParseTree<Symbol>>(a));
    }

    void reduce(const Rule& rule)
    {
        auto node = std::make_unique<ParseTree<Symbol>>(rule.first);
        std::vector<decltype(node)> children;
        for (int i = 0; i < (int)(rule.second.size()); ++i) {
            symbols.pop_back();
            children.push_back(std::move(state.back()));
            state.pop_back();
        }
        while (!children.empty()) {
            node->children.push_back(std::move(children.back()));
            children.pop_back();
        }
        symbols.push_back(rule.first);
        state.push_back(std::move(node));
    }
};
