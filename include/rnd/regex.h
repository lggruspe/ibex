#pragma once
#include "alphabet.hpp"
#include <iostream>
#include <memory>

namespace regex
{
;

enum class Type { Symbol, Union, Concatenation, Closure };

struct _Expr {
    using Expr = typename std::shared_ptr<_Expr>;

    Type type;
    std::shared_ptr<Alphabet> alphabet;
    Alphabet::Category value;

    std::shared_ptr<_Expr> left;
    std::shared_ptr<_Expr> right;

    // used to initialize regular operations
    _Expr(Type type, 
            std::shared_ptr<Alphabet> alphabet, 
            Expr left, 
            Expr right=nullptr)
        : type(type)
        , alphabet(alphabet)
        , value(Alphabet::Category(0, 0))
        , left(left)
        , right(right)
    {}

    // used to initialize leaves
    _Expr(int a, int b)
        : type(Type::Symbol)
        , alphabet(std::make_shared<Alphabet>())
        , value(Alphabet::Category(a, b))
        , left(nullptr)
        , right(nullptr)

    {
        alphabet->insert(value);
    }
};

using Expr = typename std::shared_ptr<_Expr>;

Expr operator+(Expr, Expr);
Expr operator|(Expr, Expr);
Expr closure(Expr);
Expr symbol(int, int);
Expr symbol(int);

std::ostream& operator<<(std::ostream&, Expr);
} // end namespace
