#include "regex2.h"
#include <iostream>
#include <stdexcept>

namespace regex2
{

void combine_alphabets(std::shared_ptr<Alphabet> alpha, std::shared_ptr<Alphabet>& beta)
{
    *alpha += *beta;
    beta = alpha;
}

template <class ExprContainer>
Expr _alternate(ExprContainer exprs)
{
    // idea: assume exprs is a list
    // take two at a time, and push their union at the back
    // repeat until exprs contains only one expression
    // note: assumes all the exprs are disjoint
    // (should only be called from split_leaves
    assert(!exprs.empty());
    while (exprs.size() > 1) {
        auto a = exprs.front();
        exprs.pop_front();
        auto b = exprs.front();
        exprs.pop_front();

        // construct their union
        combine_alphabets(a->alphabet, b->alphabet);
        auto c = std::make_shared<_Expr>(a->alphabet);
        c->type = Union;
        c->lhs = a;
        c->rhs = b;

        // push union at the back
        exprs.push_back(c);
    }
    auto expr = exprs.front();
    return expr;
}

void split_leaves(Expr expr, const Alphabet& alphabet)
{
    if (expr == nullptr) {
        return;
    }
    if (expr->type == Symbol) {
        Alphabet singleton;     // contains only expr->value
        singleton += expr->value;
        auto values = singleton + (singleton & alphabet);

        // get regex union of intervals in values
        // but first, turn the intervals into expressions
        std::list<Expr> exprs;
        std::transform(values.begin(), values.end(), std::back_inserter(exprs),
                [](const auto& interv) { return symbol(interv); });
       
        Expr temp = _alternate(exprs);
        // set expr to temp
        expr->type = temp->type;
        expr->value = temp->value;  // in case temp is just a symbol
        //expr->alphabet = temp->alphabet   // not needed?
        expr->lhs = temp->lhs;
        expr->rhs = temp->rhs;
    } else {
        *(expr->alphabet) = alphabet;
        split_leaves(expr->lhs, alphabet);
        split_leaves(expr->rhs, alphabet);
    }
}

Expr operator|(Expr a, Expr b)
{
    combine_alphabets(a->alphabet, b->alphabet);
    Expr c = std::make_shared<_Expr>(a->alphabet);
    c->type = Union;
    c->lhs = a;
    c->rhs = b;
    split_leaves(c, *(c->alphabet));
    return c;
}

Expr operator+(Expr a, Expr b)
{
    combine_alphabets(a->alphabet, b->alphabet);
    Expr c = std::make_shared<_Expr>(a->alphabet);
    c->type = Concatenation;
    c->lhs = a;
    c->rhs = b;
    split_leaves(c, *(c->alphabet));
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

Expr symbol(boost::icl::interval<char>::type value)
{
    Expr expr = std::make_shared<_Expr>(value);
    expr->type = Symbol;
    expr->value = value;
    expr->lhs = nullptr;
    expr->rhs = nullptr;
    return expr;
}
Expr symbol(char start, char end)
{
    // creates a class of symbols containing all chars between start and end
    // (including end)
    assert(start <= end);
    return symbol(boost::icl::interval<char>::closed(start, end));
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
