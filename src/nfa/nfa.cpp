#include "nfa/nfa.h"
#include "nfa/_nfa.h"
#include "regex2/regex2.h"
#include <boost/icl/split_interval_set.hpp>
#include <iostream>
#include <stdexcept>

namespace automata 
{

Nfa thompson(regex2::Expr expr)
{
    if (expr->type == regex2::Symbol) {
        return symbol(expr->value);
    }
    if (expr->type == regex2::Closure) {
        Nfa A = thompson(expr->lhs);
        return closure(A);
    }
    if (expr->type == regex2::Concatenation) {
        Nfa A = thompson(expr->lhs);
        return concatenate(A, thompson(expr->rhs));
    }
    if (expr->type == regex2::Union) {
        Nfa A = thompson(expr->lhs);
        return alternate(A, thompson(expr->rhs));
    }
    throw std::invalid_argument("bad expression type");
}

std::ostream& operator<<(std::ostream& out, const Nfa& nfa)
{
    out << "start: " << nfa.start << std::endl;
    out << "accept: " << nfa.accept << std::endl;
    out << "symbols: ";
    for (auto a: nfa.symbols) {
        out << (char)a << " ";
    }
    out << std::endl;
    out << "transitions:" << std::endl;

    for (auto it = nfa.delta.begin(); it != nfa.delta.end(); ++it) {
        int q = it->first;
        for (auto jt = it->second.begin(); jt != it->second.end(); ++jt) {
            auto a = jt->first;
            for (auto r: jt->second) {
                out << "(" << q << ", " << (char)a << ", " << r << ")\n";
            }
        }
    }
    out << std::endl << std::endl;
    return out;
}

} // end namespace
