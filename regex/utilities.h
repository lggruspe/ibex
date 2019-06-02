#pragma once
#include "regex.h"

namespace regex {

Expr empty()
{
    return symbol("");
}

Expr identifier()
{
    Expr letter = symbol("letter");
    Expr digit = symbol("digit");
    return letter + closure(letter | digit);
}

Expr integer()
{
    Expr zero = symbol("zero");
    Expr nonzero = symbol("nonzero");
    return zero | (nonzero + closure(zero | nonzero));
}

Expr real()
{
    Expr intgr = integer();
    Expr zero = symbol("zero");
    Expr nonzero = symbol("nonzero");
    Expr digit = zero | nonzero;

    Expr eps = empty();
    Expr decimal = eps | (symbol(".") + digit + closure(digit));
    
    Expr sign = eps | symbol("sign");
    Expr exponent = symbol("exponent") + sign + intgr;
    exponent = eps | exponent;
    return intgr + decimal + exponent;
}

Expr whitespace()
{
    return symbol("whitespace");
}

}
