#pragma once
#include "parser.h"
#include <iostream>
#include <map>
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

}
