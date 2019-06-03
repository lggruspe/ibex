#include "nfa.h"
#include "regex.h"
#include <map>
#include <memory>
#include <set>
#include <string>
#include <stdexcept>

namespace automata 
{

int merge(Nfa& A, const Nfa& B)
{
    // add states of B into this (with renamed states)
    // returns offset to B states
    std::map<int, std::map<std::string, std::set<int>>> delta;
    int offset = A.add_state();
    for (auto it = B.delta.begin(); it != B.delta.end(); ++it) {
        int q = it->first;
        for (auto jt = it->second.begin(); jt != it->second.end(); ++jt) {
                std::string a = jt->first;
            for (auto r: jt->second) {
                delta[q+offset][a].insert(r+offset);
            }
        }
    }

    for (auto it = delta.begin(); it != delta.end(); ++it) {
        int q = it->first;
        for (auto jt = it->second.begin(); jt != it->second.end(); ++jt) {
            std::string a = jt->first;
            for (auto r: jt->second) {
                A.add_transition(q, a, r);
            }
        }
    }
    return offset;
}

Nfa symbol(const std::string& a)
{
    Nfa nfa;
    int start = nfa.add_state();;
    int accept = nfa.add_state();;
    nfa.start = start;
    nfa.accept = accept;
    nfa.add_transition(start, a, accept);
    return nfa;
}

Nfa alternate(Nfa& A, const Nfa& B)
{
    // works even if A = B
    int offset = merge(A, B);
    int bs = B.start + offset;
    int bf = B.accept + offset;
    int start = A.add_state();
    int accept = A.add_state();

    A.add_transition(start, "", A.start);
    A.add_transition(start, "", bs);
    A.add_transition(A.accept, "", accept);
    A.add_transition(bf, "", accept);

    A.start = start;
    A.accept = accept;
    return A;
}

Nfa concatenate(Nfa& A, const Nfa& B)
{
    int offset = merge(A, B);
    int bs = B.start + offset;
    int bf = B.accept + offset;
    A.add_transition(A.accept, "", bs);
    A.accept = bf;
    return A;
}

Nfa closure(Nfa& A)
{
    int start = A.add_state();
    int accept = A.add_state();
    A.add_transition(start, "", A.start);
    A.add_transition(start, "", accept);
    A.add_transition(A.accept, "", A.start);
    A.add_transition(A.accept, "", accept);
    A.start = start;
    A.accept = accept;
    return A;
}

Nfa thompson(std::weak_ptr<regex::Tree> expr)
{
    if (expr.expired()) {
        throw "regex weak_ptr expired";
    }
    auto sp = expr.lock();
    if (sp->lhs == nullptr) {
        return symbol(sp->value);
    }
    if (sp->value == "*") {
        Nfa A = thompson(sp->lhs);
        return closure(A);
    }
    if (sp->rhs == nullptr || (sp->value != "+" && sp->value != "|")) {
        throw "malformed regular expression";
    }
    Nfa A = thompson(sp->lhs);
    Nfa B = thompson(sp->rhs);
    return sp->value == "+" ? concatenate(A, B) : alternate(A, B);
}

//Nfa thompson(regex2::Expr expr)
//{
//    switch (expr->type) {
//    case Symbol:
//    case Concatenation:
//    case Closure:
//    case Union:
//    default:
//        throw std::invalid_argument("bad expression type");
//    }
//}

} // end namespace
