#pragma once
#include <memory>

#include <algorithm>
#include <stdexcept>
#include <list>
#include <boost/icl/split_interval_set.hpp>
#include <cassert>
#include <iostream>

namespace regex2
{

using Alphabet = typename boost::icl::split_interval_set<char>;

enum ExprType { Symbol, Union, Concatenation, Closure };

class _Expr {
    using Expr = typename std::shared_ptr<_Expr>;
    using CharInterval = typename boost::icl::interval<char>;
    using Type = ExprType;

public:
    Type type;
    std::shared_ptr<Alphabet> alphabet;
    CharInterval::type value;   // only access if type == Symbol

    std::shared_ptr<_Expr> lhs;
    std::shared_ptr<_Expr> rhs;
    _Expr(std::shared_ptr<Alphabet> alphabet) : alphabet(alphabet) {}

    _Expr(CharInterval::type value) : value(value)
    {
        alphabet = std::make_shared<Alphabet>();
        *alphabet += value;
    }
};

using Expr = typename std::shared_ptr<_Expr>;   // segmentation fault when initialized without value

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
