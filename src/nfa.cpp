#include "alphabet.hpp"
#include "nfa.h"
#include "regex.h"
#include <boost/icl/split_interval_set.hpp>
#include <cassert>
#include <iostream>
#include <map>
#include <set>
#include <stdexcept>

#define gotoifn(label, condition) do {\
    if (!(condition)) {\
        std::cerr << #condition << std::endl;\
        goto label;\
    }\
} while (0)

namespace automata 
{

int add_state(Nfa&, int);
int add_state(Nfa&);
void add_transition(Nfa&, int, int, Alphabet::Category);
Nfa thompson(regex::Expr);

Nfa::Nfa(Alphabet::Category a)
{
    start = add_state(*this);;
    accept = add_state(*this);;
    add_transition(*this, start, accept, a);
}

Nfa::Nfa(regex::Expr expr)
{
    *this = thompson(expr);
}

// add state with number q
int add_state(Nfa& nfa, int q)
{
    nfa.delta[q];
    return q;
}

// add state and returns number of the new state
int add_state(Nfa& nfa)
{
    int q = nfa.delta.empty() ? 0 : nfa.delta.rbegin()->first + 1;
    return add_state(nfa, q);
}

void add_transition(Nfa& nfa, int q, int r, Alphabet::Category a=Alphabet::Category(0, 0))
{
    add_state(nfa, q);
    add_state(nfa, r);
    nfa.delta[q][a].insert(r);
    if (a) {
        nfa.symbols.insert(a);
    }
}

// add states of B into this (with renamed states)
// returns offset to B states
int merge(Nfa& A, const Nfa& B)
{
    std::map<int, std::map<Alphabet::Category, std::set<int>>> delta;
    int offset = add_state(A);
    for (auto it = B.delta.begin(); it != B.delta.end(); ++it) {
        int q = it->first;
        for (auto jt = it->second.begin(); jt != it->second.end(); ++jt) {
            auto a = jt->first;
            for (auto r: jt->second) {
                delta[q+offset][a].insert(r+offset);
            }
        }
    }

    for (auto it = delta.begin(); it != delta.end(); ++it) {
        int q = it->first;
        for (auto jt = it->second.begin(); jt != it->second.end(); ++jt) {
            auto a = jt->first;
            for (auto r: jt->second) {
                add_transition(A, q, r, a);
            }
        }
    }
    return offset;
}

Nfa symbol(Alphabet::Category a)
{
    return Nfa(a);
}

Nfa alternate(Nfa& A, const Nfa& B)
{
    // works even if A = B
    int offset = merge(A, B);
    int bs = B.start + offset;
    int bf = B.accept + offset;
    int start = add_state(A);
    int accept = add_state(A);

    add_transition(A, start, A.start);
    add_transition(A, start, bs);
    add_transition(A, A.accept, accept);
    add_transition(A, bf, accept);

    A.start = start;
    A.accept = accept;
    return A;
}

Nfa concatenate(Nfa& A, const Nfa& B)
{
    int offset = merge(A, B);
    int bs = B.start + offset;
    int bf = B.accept + offset;
    add_transition(A, A.accept, bs);
    A.accept = bf;
    return A;
}

Nfa closure(Nfa& A)
{
    int start = add_state(A);
    int accept = add_state(A);
    add_transition(A, start, A.start);
    add_transition(A, start, accept);
    add_transition(A, A.accept, A.start);
    add_transition(A, A.accept, accept);
    A.start = start;
    A.accept = accept;
    return A;
}

Nfa thompson(regex::Expr expr)
{
    if (expr->type == regex::Type::Symbol) {
        return symbol(expr->value);
    }

    gotoifn(error, expr->left);
    if (expr->type == regex::Type::Closure) {
        Nfa A = thompson(expr->left);
        return closure(A);
    }

    gotoifn(error, expr->right);

    if (expr->type == regex::Type::Concatenation) {
        Nfa A = thompson(expr->left);
        return concatenate(A, thompson(expr->right));
    }
    if (expr->type == regex::Type::Union) {
        Nfa A = thompson(expr->left);
        return alternate(A, thompson(expr->right));
    }

error:
    std::cerr << "DEBUG: " << expr << std::endl;
    std::cerr << std::endl;
    throw std::invalid_argument("bad expression type");
}

std::ostream& operator<<(std::ostream& out, const Nfa& nfa)
{
    out << "start: " << nfa.start << std::endl;
    out << "accept: " << nfa.accept << std::endl;
    out << "symbols: ";
    for (auto a: nfa.symbols) {
        out << "[" << a.start << ", " << a.end << "] ";
    }
    out << std::endl;
    out << "transitions:" << std::endl;

    for (auto it = nfa.delta.begin(); it != nfa.delta.end(); ++it) {
        int q = it->first;
        for (auto jt = it->second.begin(); jt != it->second.end(); ++jt) {
            auto a = jt->first;
            for (auto r: jt->second) {
                out << "(" << q << ", " << "[" << a.start << ", " << a.end << "] " << ", " << r << ")\n";
            }
        }
    }
    out << std::endl << std::endl;
    return out;
}

} // end namespace
