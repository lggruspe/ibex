#pragma once

#include "cats.h"
#include <memory>
#include <string>

#include <cassert>
#include <iostream>

namespace regex2
{

class _Expr {
    using Expr = typename std::shared_ptr<_Expr>;
public:
    Alphabet alphabet;  // TODO use pointer instead
    std::string value;
    std::shared_ptr<_Expr> lhs;
    std::shared_ptr<_Expr> rhs;
    _Expr(const Alphabet& alphabet, const std::string& value="", 
            Expr lhs=nullptr, Expr rhs=nullptr)
        : alphabet(alphabet), value(value), lhs(lhs), rhs(rhs) {}
};

using Expr = typename std::shared_ptr<_Expr>;

Expr operator|(Expr a, Expr b)
{
    return std::make_shared<_Expr>(a->alphabet + b->alphabet, "|", a, b);
}

Expr operator+(Expr a, Expr b)
{
    return std::make_shared<_Expr>(a->alphabet + b->alphabet, "+", a, b);
}

Expr closure(Expr a)
{
    return std::make_shared<_Expr>(a->alphabet, "*", a);
}

Expr symbol(char start, char end)
{
    // creates a class of symbols containing all chars between start and end
    // (including end)
    assert(start <= end);
    Alphabet alphabet;
    alphabet.insert(start, end);
    std::string value = "[";
    value += start + std::string(1, '-') + end;
    value += "]";
    return std::make_shared<_Expr>(alphabet, value);
    // TODO symbol value doesn't change even when the corresponding interval
    // gets split
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
