#include "automaton.hpp"
#include "nexpr.hpp"
#include "zsymbols.hpp"
#include <iostream>

std::ostream& operator<<(std::ostream& os, const NExpr& expr)
{
    for (const auto& [q, dq]: expr.states) {
        os << "q = " << q << std::endl;
        for (const auto& [a, R]: dq) {
            os << "[" << a.start << ", " << a.end << "): ";
            for (const auto& r: R) {
                os << r << " ";
            }
            os << std::endl;
        }
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const Automaton& fsm)
{
    for (const auto& [q, dq]: fsm.states) {
        for (const auto& [a, r]: dq) {
            os << "d(" << q << ", [" << a.start << ", " << a.end << ")) = " << r << std::endl;
        }
    }
    os << "accepts: ";
    for (const auto& f: fsm.accepts) {
        os << f << " ";
    }
    os << "\nerror: " << fsm.error;
    return os;
}

NExpr optional(const NExpr& expr)
{
    return alternate(expr, NExpr(ZRange()));
}

NExpr integer()
{
    ZRange nonzero('1', '9');
    ZRange zero('0', '0');
    ZRange digit('0', '9');
    return alternate(zero, concatenate(nonzero, closure(digit)));
}

NExpr number()
{
    ZRange digit('0', '9');
    ZRange dot('.', '.');
    auto decimal = concatenate(dot, concatenate(digit, closure(digit)));

    ZRange e('e', 'e');
    ZRange E('E', 'E');
    ZRange plus('+', '+');
    ZRange minus('-', '-');
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

NExpr identifier()
{
    ZRange small('a', 'z');
    ZRange big('A', 'Z');
    ZRange under('_', '_');
    ZRange digit('0', '9');
    auto front = alternate(small, alternate(big, under));
    return concatenate(front, closure(alternate(front, digit)));
}

NExpr whitespace()
{
    ZRange tab('\t', '\t');
    ZRange nl('\n', '\n');
    ZRange space(' ', ' ');
    return alternate(tab, alternate(nl, space));
}

NExpr character()
{
    ZRange quote('\'', '\'');
    ZRange backslash('\\', '\\');
    ZRange t('t', 't');
    ZRange n('n', 'n');
    auto escape = concatenate(backslash, 
        alternate(
            quote, 
            alternate(
                backslash, 
                alternate(
                    t, 
                    n))));
    auto middle = alternate(
        ZRange(32, 38),
        alternate(
            ZRange(40, 91),
            alternate(
                ZRange(93, 126),
                escape)));
    return concatenate(quote, concatenate(middle, quote));
}

NExpr string()
{
    ZRange quotes('"', '"');
    ZRange backslash('\\', '\\');
    auto middle = closure(
        alternate(
            ZRange(32, 33),
            alternate(
                ZRange(35, 91),
                alternate(
                    ZRange(93, 126),
                    concatenate(
                        backslash,
                        ZRange(32, 126))))));
    return concatenate(quotes, concatenate(middle, quotes));
}

#define M_PRINT_CONVERT(Expr_) do { \
    auto expr = Expr_(); \
    std::cout << #Expr_ << std::endl; \
    std::cout << expr << std::endl; \
    std::cout << Automaton(expr) << std::endl; \
} while (0)

int main()
{
    M_PRINT_CONVERT(identifier);
    M_PRINT_CONVERT(string);
    M_PRINT_CONVERT(character);
    M_PRINT_CONVERT(number);
}
