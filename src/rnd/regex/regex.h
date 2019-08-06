#pragma once
#include "rnd/alphabet.hpp"
#include <iostream>
#include <memory>

namespace regex
{
;

enum class Type { Symbol, Union, Concatenation, Closure };

struct ExpTree {
    using Expr = typename std::shared_ptr<ExpTree>;

    Type type;
    std::shared_ptr<Alphabet> alphabet;
    Alphabet::Category value;

    std::shared_ptr<ExpTree> left;
    std::shared_ptr<ExpTree> right;

    // used to initialize regular operations
    ExpTree(Type type, 
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
    ExpTree(int a, int b)
        : type(Type::Symbol)
        , alphabet(std::make_shared<Alphabet>())
        , value(Alphabet::Category(a, b))
        , left(nullptr)
        , right(nullptr)

    {
        alphabet->insert(value);
    }
};

using Expr = typename std::shared_ptr<ExpTree>;

Expr operator+(Expr, Expr);
Expr operator|(Expr, Expr);
Expr closure(Expr);
Expr symbol(int, int);
Expr symbol(int);

std::ostream& operator<<(std::ostream&, Expr);
} // end namespace
