#pragma once
#include "regex.h"
#include <exception>

namespace re {
    Re empty()
    {
        return symbol("");
    }

    Re identifier()
    {
        Re letter = symbol("letter");
        Re digit = symbol("digit");
        return concatenate(letter, closure(alternate(letter, digit)));
    }

    Re integer()
    {
        Re zero = symbol("zero");
        Re nonzero = symbol("nonzero");
        return alternate(zero, concatenate(nonzero, closure(alternate(zero, nonzero))));
    }

    Re real()
    {
        Re intgr = integer();
        Re zero = symbol("zero");
        Re nonzero = symbol("nonzero");
        Re digit = alternate(zero, nonzero);

        Re eps = empty();
        Re decimal = alternate(eps, concatenate(symbol("."), concatenate(digit, closure(digit))));
        
        Re sign = alternate(eps, symbol("sign"));
        Re exponent = concatenate(symbol("exponent"), concatenate(sign, intgr));
        return concatenate(intgr, concatenate(decimal, exponent));
    }
}
