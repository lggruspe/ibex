#pragma once

#include "cats.h"
#include <memory>

#include <algorithm>
#include <stdexcept>
#include <list>

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

template <class ExprContainer>
Expr _alternate(ExprContainer);

void split_leaves(Expr, const Alphabet&);
Expr operator+(Expr, Expr);
Expr operator|(Expr, Expr);
Expr closure(Expr);
Expr symbol(boost::icl::interval<char>::type);
Expr symbol(char, char);
Expr symbol(char);

std::ostream& operator<<(std::ostream&, Expr);
} // end namespace
