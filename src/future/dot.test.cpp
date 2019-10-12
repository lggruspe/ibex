#include "dfa.hpp"
#include "dotify.hpp"
#include <iostream>
#include <string>

Expr optional(const Expr& expr)
{
    return alternate(expr, Expr(ClosedInterval()));
}

Expr integer()
{
    ClosedInterval nonzero('1', '9');
    ClosedInterval zero('0', '0');
    ClosedInterval digit('0', '9');
    return alternate(zero, concatenate(nonzero, closure(digit)));
}

Expr number()
{
    ClosedInterval digit('0', '9');
    ClosedInterval dot('.', '.');
    auto decimal = concatenate(dot, concatenate(digit, closure(digit)));

    ClosedInterval e('e', 'e');
    ClosedInterval E('E', 'E');
    ClosedInterval plus('+', '+');
    ClosedInterval minus('-', '-');
    auto exponent = concatenate(
        alternate(e, E),
        concatenate(
            optional(alternate(plus, minus)),
            integer()));
    return concatenate(
        integer(),
        concatenate(
            optional(decimal),
            optional(exponent)));
}

Expr identifier()
{
    ClosedInterval small('a', 'z');
    ClosedInterval big('A', 'Z');
    ClosedInterval under('_', '_');
    ClosedInterval digit('0', '9');
    auto front = alternate(small, alternate(big, under));
    return concatenate(front, closure(alternate(front, digit)));
}

Expr whitespace()
{
    ClosedInterval tab('\t', '\t');
    ClosedInterval nl('\n', '\n');
    ClosedInterval space(' ', ' ');
    return alternate(tab, alternate(nl, space));
}

Expr character()
{
    ClosedInterval quote('\'', '\'');
    ClosedInterval backslash('\\', '\\');
    ClosedInterval t('t', 't');
    ClosedInterval n('n', 'n');
    auto escape = concatenate(backslash, 
        alternate(
            quote, 
            alternate(
                backslash, 
                alternate(
                    t, 
                    n))));
    auto middle = alternate(
        ClosedInterval(32, 38),
        alternate(
            ClosedInterval(40, 91),
            alternate(
                ClosedInterval(93, 126),
                escape)));
    return concatenate(quote, concatenate(middle, quote));
}

Expr string()
{
    ClosedInterval quotes('"', '"');
    ClosedInterval backslash('\\', '\\');
    auto middle = closure(
        alternate(
            ClosedInterval(32, 33),
            alternate(
                ClosedInterval(35, 91),
                alternate(
                    ClosedInterval(93, 126),
                    concatenate(
                        backslash,
                        ClosedInterval(32, 126))))));
    return concatenate(quotes, concatenate(middle, quotes));
}

#define M_PRINT_CONVERT(Expr_) do { \
    auto expr = Expr_(); \
    std::cout << dotify(Fsm(expr)) << std::endl; \
} while (0)

int main(int argc, char **argv)
{
    if (argc != 2) {
        M_PRINT_CONVERT(identifier);
        M_PRINT_CONVERT(string);
        M_PRINT_CONVERT(character);
        M_PRINT_CONVERT(number);
        return 0;
    }
    if (std::string input = argv[1]; input == "identifier") {
        M_PRINT_CONVERT(identifier);
    } else if (input == "string") {
        M_PRINT_CONVERT(string);
    } else if (input == "character") {
        M_PRINT_CONVERT(character);
    } else if (input == "number") {
        M_PRINT_CONVERT(number);
    }
}
