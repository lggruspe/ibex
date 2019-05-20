#pragma once
#include <iostream>
#include <memory>
#include <string>

namespace regex {

class Tree {
    typedef std::shared_ptr<Tree> Expr;
public:
    std::string value;
    Expr lhs, rhs;
    Tree(const std::string& symbol="", Expr lhs=nullptr, Expr rhs=nullptr)
    {
        this->value = symbol;
        this->lhs = lhs;
        this->rhs = rhs;
    }
};

typedef std::shared_ptr<Tree> Expr;

Expr operator|(Expr, Expr);
Expr operator+(Expr, Expr);
Expr closure(Expr);
Expr symbol(const std::string&);
std::ostream& operator<<(std::ostream&, std::weak_ptr<Tree>);

std::string to_string(std::weak_ptr<Tree>);

} // end namespace
