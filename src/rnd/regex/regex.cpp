#include "regex.h"
#include <cassert>
#include <iostream>
#include <list>
#include <stdexcept>

namespace rnd
{
namespace regex
{

Expr symbol(SymbolRange);

void combine_alphabets(std::shared_ptr<Alphabet> alpha, std::shared_ptr<Alphabet>& beta)
{
    alpha->combine(*beta);
    beta = alpha;
}

// idea: pop 2 at the front, push 1 at the back, until there's only expr
Expr n_ary_union(std::list<Expr>& exprs)
{
    assert(!exprs.empty());
    while (exprs.size() > 1) {
        auto a = exprs.front();
        exprs.pop_front();
        auto b = exprs.front();
        exprs.pop_front();
        auto c = std::make_shared<ExpTree>(Type::Union, a->alphabet, a, b);
        exprs.push_back(c);
    }
    return exprs.front();
}

void make_leaves_disjoint(Expr expr)
{
    if (!expr) {
        return;
    }
    if (expr->type != Type::Symbol) {
        if (expr->left) {
            expr->left->alphabet = expr->alphabet;
            make_leaves_disjoint(expr->left);
        }
        if (expr->right) {
            expr->right->alphabet = expr->alphabet;
            make_leaves_disjoint(expr->right);
        }
    } else {
        // get all intervals in the alphabet that intersect with symbol
        auto node = expr->alphabet->first_overlap(expr->value.start, expr->value.end);
        assert(!rb::predecessor(node) || !overlaps(rb::predecessor(node)->data, expr->value));
        std::list<Expr> overlaps;
        while (node && ::rnd::overlaps(node->data, expr->value)) {
            auto new_leaf = symbol(node->data.start, node->data.end);
            new_leaf->alphabet = expr->alphabet;
            overlaps.push_back(new_leaf);
            node = rb::successor(node); // TODO abstraction leak
        }

        // get the union of all these intervals
        expr = n_ary_union(overlaps);
    }
}

Expr operator|(Expr a, Expr b)
{
    combine_alphabets(a->alphabet, b->alphabet);
    Expr c = std::make_shared<ExpTree>(Type::Union, a->alphabet, a, b);
    make_leaves_disjoint(c);
    return c;
}

Expr operator+(Expr a, Expr b)
{
    combine_alphabets(a->alphabet, b->alphabet);
    Expr c = std::make_shared<ExpTree>(Type::Concatenation, a->alphabet, a, b);
    make_leaves_disjoint(c);
    return c;
}

Expr closure(Expr a)
{
    return std::make_shared<ExpTree>(Type::Closure, a->alphabet, a);
}

Expr symbol(int start, int end)
{
    return std::make_shared<ExpTree>(start, end);
}

Expr symbol(int start)
{
    return symbol(start, start);
}

std::ostream& operator<<(std::ostream& out, Expr re)
{
    if (re == nullptr) {
        return (out << "null");
    }
    switch (re->type) {
    case Type::Symbol:    
        return out << re->value;
    case Type::Union:
        return out << "(union, " << re->left << ", " << re->right << ")";
    case Type::Concatenation:
        return out << "(concatenation, " << re->left << ", " << re->right << ")";
    case Type::Closure:
        return out << "(closure, " << re->left << ")";
    default:
        throw std::invalid_argument("input expression has bad type");
    }
}

} // end namespace
}
