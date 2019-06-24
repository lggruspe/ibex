#include "nfa.h"
#include "regex2/regex2.h"
#include <boost/icl/split_interval_set.hpp>
#include <iostream>
#include <map>
#include <set>
#include <stdexcept>

namespace automata 
{

int add_state(Nfa&, int);
int add_state(Nfa&);
void add_transition(Nfa&, int, int, char);
Nfa thompson(regex2::Expr);

Nfa::Nfa(char a)
{
    start = add_state(*this);;
    accept = add_state(*this);;
    add_transition(*this, start, accept, a);
}

Nfa::Nfa(regex2::Expr expr)
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

void add_transition(Nfa& nfa, int q, int r, char a='\0')
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
    std::map<int, std::map<char, std::set<int>>> delta;
    int offset = add_state(A);
    for (auto it = B.delta.begin(); it != B.delta.end(); ++it) {
        int q = it->first;
        for (auto jt = it->second.begin(); jt != it->second.end(); ++jt) {
                char a = jt->first;
            for (auto r: jt->second) {
                delta[q+offset][a].insert(r+offset);
            }
        }
    }

    for (auto it = delta.begin(); it != delta.end(); ++it) {
        int q = it->first;
        for (auto jt = it->second.begin(); jt != it->second.end(); ++jt) {
            char a = jt->first;
            for (auto r: jt->second) {
                add_transition(A, q, r, a);
            }
        }
    }
    return offset;
}

Nfa symbol(char a)
{
    return Nfa(a);
}

Nfa symbol(boost::icl::interval<char>::type interval)
{
    char a = interval.lower();
    // check if interval is left open
    if (!boost::icl::contains(interval, a)) {
        ++a;
    }
    // check if a is still in the interval
    if (!boost::icl::contains(interval, a)) {
        a = '\0';
    }
    return symbol(a);
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
