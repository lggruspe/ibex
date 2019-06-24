#include "utilities.h"
#include "regex.h"

namespace regex
{

Expr empty()
{
    return symbol('\0');
}

Expr optional(Expr a)
{
    return empty() | a;
}

Expr integer()
{
    auto nonzero = symbol('1', '9');
    auto digit = symbol('0', '9');
    return symbol('0') | (nonzero + closure(digit));
}

Expr number()
{
    auto digit = symbol('0', '9');
    auto decimal = symbol('.') + digit + closure(digit);

    auto exponent = (symbol('e') | symbol('E'))
        + optional(symbol('-') | symbol('+'))
        + integer();
        
    return integer() + optional(decimal) + optional(exponent);
}

Expr identifier()
{
    auto letter = symbol('_') | symbol('a', 'z') | symbol('A', 'Z');
    return letter + closure(letter | symbol('0', '9'));
}

Expr whitespace()
{
    return symbol(' ') | symbol('\t') | symbol('\n');
}

Expr character()
{
    Expr escape = symbol('\\') + (symbol('\'') | symbol('\\') | symbol('t') | symbol('n'));
    Expr middle = symbol(32, 38) | symbol(40, 91) | symbol(93, 126) | escape;
    return symbol('\'') + middle + symbol('\'');
}


Expr string()
{
    return symbol('"') + closure((symbol(32, 33) | symbol(35, 91) | symbol(93, 126))
            | (symbol('\\') + symbol(32, 126))) + symbol('"');
}

} // end namespace
