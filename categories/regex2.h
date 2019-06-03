#pragma once

#include "cats.h"
#include <memory>

#include <stdexcept>

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

void split_leaves(Expr expr, const Alphabet& alphabet)
{
    assert(expr != nullptr);

    if (expr.type == Symbol) {
        // TODO continue here
        Alphabet A; // subset of alphabet whose intervals intersect with expr->value
        // TODO continue here
    } else {
        expr->alphabet = alphabet;
        split_leaves(expr->lhs, alphabet);
        split_leaves(expr->rhs, alphabet);
    }
}

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
    insert(alphabet, start, end);
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
    switch (re->type) {
    case Symbol:    
        return (out << re->value);
    case Union:
        return out << "(union, " << re->lhs << ", " << re->rhs << ")";
    case Concatenation:
        return out << "(concatenation, " << re->lhs << ", " << re->rhs << ")";
    case Closure:
        return out << "(closure, " << re->lhs << ")";
    default:
        throw std::invalid_argument("input expression has bad type");
    }
}

} // end namespace
