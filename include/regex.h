#pragma once
#include <boost/icl/split_interval_set.hpp>
#include <iostream>
#include <memory>

namespace regex
{

using Alphabet = typename boost::icl::split_interval_set<char>;

enum class Type { Symbol, Union, Concatenation, Closure };

class _Expr {
    using Expr = typename std::shared_ptr<_Expr>;
    using CharInterval = typename boost::icl::interval<char>;

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

Expr operator+(Expr, Expr);
Expr operator|(Expr, Expr);
Expr closure(Expr);
Expr symbol(char, char);
Expr symbol(char);

std::ostream& operator<<(std::ostream&, Expr);
} // end namespace
