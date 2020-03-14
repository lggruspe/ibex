#pragma once
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

struct ParseTree {
    using Tree = std::unique_ptr<ParseTree>;
    std::vector<Tree> children;
    const std::string label;
    const std::string value;

    ParseTree(const std::string& label, const std::string& value = "")
        : label(label)
        , value(value)
    {}
};

struct SyntaxError {
    char const* what() const { return "syntax error"; }
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

struct ParseTreeCallback {
    using Rule = std::pair<std::string, std::vector<std::string>>;
    std::vector<std::unique_ptr<ParseTree>> state;

    auto accept(bool acc)
    {
        if (!acc) {
            throw SyntaxError();
        }
        return std::move(state.back());
    }

    void shift(const std::pair<std::string, std::string>& lookahead)
    {
        const auto& [tok, lex] = lookahead;
        state.push_back(std::make_unique<ParseTree>(tok, lex));
    }

    void reduce(const Rule& rule)
    {
        auto node = std::make_unique<ParseTree>(rule.first);
        std::vector<decltype(node)> children;
        for (int i = 0; i < (int)(rule.second.size()); ++i) {
            children.push_back(std::move(state.back()));
            state.pop_back();
        }
        while (!children.empty()) {
            node->children.push_back(std::move(children.back()));
            children.pop_back();
        }
        state.push_back(std::move(node));
    }
};
