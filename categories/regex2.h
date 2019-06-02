#pragma once

#include "cats.h"
#include <memory>

#include <cassert>
#include <iostream>

namespace regex2
{

enum ExprType { Symbol, Union, Concatenation, Closure };

class _Expr {
    using Expr = typename std::shared_ptr<_Expr>;
    using CharInterval = typename boost::icl::interval<char>;
    using Type = ExprType;

public:
    Type type;
    Alphabet alphabet;  // TODO use pointer instead
    CharInterval::type value;   // only access if type == Symbol

    std::shared_ptr<_Expr> lhs;
    std::shared_ptr<_Expr> rhs;
    _Expr(const Alphabet& alphabet) : alphabet(alphabet) {}
};

using Expr = typename std::shared_ptr<_Expr>;

Expr operator|(Expr a, Expr b)
{
    Expr c = std::make_shared<_Expr>(a->alphabet + b->alphabet);
    c->type = Union;
    c->lhs = a;
    c->rhs = b;
    return c;
}

Expr operator+(Expr a, Expr b)
{
    Expr c = std::make_shared<_Expr>(a->alphabet + b->alphabet);
    c->type = Concatenation;
    c->lhs = a;
    c->rhs = b;
    return c;
}

Expr closure(Expr a)
{
    Expr c = std::make_shared<_Expr>(a->alphabet);
    c->type = Closure;
    c->lhs = a;
    c->rhs = nullptr;
    return c;
}

Expr symbol(char start, char end)
{
    // creates a class of symbols containing all chars between start and end
    // (including end)
    assert(start <= end);
    Alphabet alphabet;
    alphabet.insert(start, end);
    Expr expr = std::make_shared<_Expr>(alphabet);
    expr->type = Symbol;
    expr->value = boost::icl::interval<char>::closed(start, end);
    expr->lhs = nullptr;
    expr->rhs = nullptr;
    return expr;
}

Expr symbol(char start)
{
    return symbol(start, start);
}

std::ostream& operator<<(std::ostream& out, Expr re)
{
    if (re == nullptr) {
        return (out << "null");
    }
    out << "(" << re->value << ", ";
    out << re->lhs << ", " << re->rhs << ")";
    return out;
}

} // end namespace
