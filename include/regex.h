#pragma once
#include "distree.hpp"
#include <iostream>
#include <memory>

namespace regex
{
;

// wraps distree::DisSet
struct Alphabet {
    distree::DisSet set;
    using Category = typename distree::Interval;

    void insert(const Category& interval)
    {
        insert(interval.start, interval.end);
    }

    void insert(int a, int b)
    {
        Category interval(a, b);
        set.insert(a, b);
    }

    void combine(const Alphabet& other)
    {
        set.combine(other.set);
    }

    distree::DisTree* begin() const
    {
        return set.tree;
    }

    // returns leftmost interval that overlaps with interval
    distree::DisTree* first_overlap(Category interval) const
    {
        return set.first_overlap(interval);
    }
};

enum class Type { Symbol, Union, Concatenation, Closure };

class _Expr {
    using Expr = typename std::shared_ptr<_Expr>;

public:
    Type type;
    std::shared_ptr<Alphabet> alphabet;
    Alphabet::Category value;

    std::shared_ptr<_Expr> left;
    std::shared_ptr<_Expr> right;

    // used to initialize regular operations
    _Expr(Type type, 
            std::shared_ptr<Alphabet> alphabet, 
            Expr left=nullptr, 
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
