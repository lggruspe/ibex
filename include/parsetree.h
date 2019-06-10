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

    _ParseTree(std::shared_ptr<_ParseTree> parent) : parent(parent) {}
};

using ParseTree = std::shared_ptr<_ParseTree>;
std::vector<ParseTree> stack;

template <class Token>
void shift_handler(const Token& token, const std::string& lexeme)
{
}

template <class Token, class Variable>
void reduce_handler(const _ParseTree<Token, Variable>::Rule& rule)
{
}

ParseTree get_parse_tree()
{
    return stack.front();
}
