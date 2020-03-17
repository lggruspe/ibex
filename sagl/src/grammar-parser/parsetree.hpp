#pragma once
#include <cassert>
#include <iostream>
#include <map>
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

static inline std::ostream& operator<<(
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
    using Sentence = std::vector<std::string>;
    using Rule = std::pair<std::string, Sentence>;
    std::vector<std::unique_ptr<ParseTree>> state;
    std::vector<Rule> rules;

    auto accept(bool acc)
    {
        if (!acc) {
            throw SyntaxError();
        }
        std::multimap<std::string, Sentence> grammar_rules;
        for (const auto& rule: rules) {
            grammar_rules.insert(rule);
        }
        return std::make_pair(rules[0].first, grammar_rules);
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

        append_rule(rule);
    }

    void append_rule(const Rule& rule)
    {
        if (rule.first == "Rule") {
            if (rule.second == Sentence{"Lhs", "arrow", "Rhs", "dot"}) {
                assert(state.back()->children.size() == 4);
                auto* lhs = state.back()->children[0].get();
                auto* rhs = state.back()->children[2].get();
                assert(lhs && lhs->label == "Lhs");
                assert(rhs && rhs->label == "Rhs");
                rules.push_back(Rule(get_lhs(lhs), fold(rhs)));
            }
        }
    }

private:
    Sentence fold(ParseTree* t) const
    {
        if (t->children.empty()) {
            return Sentence();
        }
        assert(t->children.size() == 2);
        assert(t->children[0]->label == "Rhs");
        assert(t->children[1]->label == "identifier");
        Sentence current = fold(t->children[0].get());
        current.push_back(t->children[1]->value);
        return current;
    }

    std::string get_lhs(ParseTree* lhs) const
    {
        assert(lhs->label == "Lhs");
        assert(lhs->children.size() == 1);
        auto* child = lhs->children.front().get();
        assert(child->label == "identifier");
        assert(child->children.empty());
        return child->value;
    }
};
