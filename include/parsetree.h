#pragma once
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <variant>
#include <vector>

template <class Token, class Variable>
struct _ParseTree {
    using Symbol = std::variant<Token, Variable>;
    using Sentence = std::vector<Symbol>;
    using Rule = std::pair<Variable, Sentence>;

    std::shared_ptr<_ParseTree> parent;
    std::vector<std::shared_ptr<_ParseTree>> children;
    std::map<std::string, std::string> attributes;
    
    _ParseTree(const Rule& rule, const std::vector<std::shared_ptr<_ParseTree>>& children)
    {
        make_node(rule, children);
    }

    _ParseTree(const std::pair<Token, std::string>& word)
    {
        make_node(word);
    }

private:
    void make_node(const Rule&, const std::vector<std::shared_ptr<_ParseTree>>&);
    void make_node(const std::pair<Token, std::string>&);
};
