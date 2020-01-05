#include "automaton.hpp"
#include "nexpr.hpp"
#include "zsymbols.hpp"
#include <iostream>

using namespace rnd;

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
    ZRange nonzero('1', '9'+1);
    ZRange zero('0', '0'+1);
    ZRange digit('0', '9'+1);
    return alternate(zero, concatenate(nonzero, closure(digit)));
}

NExpr number()
{
    ZRange digit('0', '9'+1);
    ZRange dot('.', '.'+1);
    auto decimal = concatenate(dot, concatenate(digit, closure(digit)));

    ZRange e('e', 'e'+1);
    ZRange E('E', 'E'+1);
    ZRange plus('+', '+'+1);
    ZRange minus('-', '-'+1);
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
    ZRange small('a', 'z'+1);
    ZRange big('A', 'Z'+1);
    ZRange under('_', '_'+1);
    ZRange digit('0', '9'+1);
    auto front = alternate(small, alternate(big, under));
    return concatenate(front, closure(alternate(front, digit)));
}

NExpr whitespace()
{
    ZRange tab('\t', '\t'+1);
    ZRange nl('\n', '\n'+1);
    ZRange space(' ', ' '+1);
    return alternate(tab, alternate(nl, space));
}

NExpr character()
{
    ZRange quote('\'', '\''+1);
    ZRange backslash('\\', '\\'+1);
    ZRange t('t', 't'+1);
    ZRange n('n', 'n'+1);
    auto escape = concatenate(backslash, 
        alternate(
            quote, 
            alternate(
                backslash, 
                alternate(
                    t, 
                    n))));
    auto middle = alternate(
        ZRange(32, 38+1),
        alternate(
            ZRange(40, 91+1),
            alternate(
                ZRange(93, 126+1),
                escape)));
    return concatenate(quote, concatenate(middle, quote));
}

NExpr string()
{
    ZRange quotes('"', '"'+1);
    ZRange backslash('\\', '\\'+1);
    auto middle = closure(
        alternate(
            ZRange(32, 33+1),
            alternate(
                ZRange(35, 91+1),
                alternate(
                    ZRange(93, 126+1),
                    concatenate(
                        backslash,
                        ZRange(32, 126+1))))));
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
