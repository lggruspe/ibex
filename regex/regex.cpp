#include "regex.h"
#include <iostream>
#include <memory>
#include <string>

namespace regex
{

Expr operator|(Expr a, Expr b)
{
    return std::make_shared<Tree>("|", a, b);
}

Expr operator+(Expr a, Expr b)
{
    return std::make_shared<Tree>("+", a, b);
}

Expr closure(Expr a)
{
    return std::make_shared<Tree>("*", a);
}

Expr symbol(const std::string& s)
{
    return std::make_shared<Tree>(s);
}

std::string to_string(std::weak_ptr<Tree> a)
{
    if (a.expired()) {
        return "";
    }
    auto sp = a.lock();
    if (sp->lhs == nullptr) {
        return sp->value;
    }
    if (sp->rhs == nullptr) {
        return "(" + sp->value + ", " + to_string(sp->lhs) + ")";
    }
    return "(" + sp->value + ", " + to_string(sp->lhs) + ", " + to_string(sp->rhs) + ")";
}

std::ostream& operator<<(std::ostream& os, std::weak_ptr<Tree> a)
{
    os << to_string(a);
    return os;
}

} // end namespace
