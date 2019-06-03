namespace regex2
{
;

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

}   // end namespace
