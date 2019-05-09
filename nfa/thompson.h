#pragma once
#include "nfa.h"
#include "../regex/regex.h"

namespace automata {
    Nfa thompson(std::weak_ptr<regex::RegexTree> expr)
    {
        if (expr.expired()) {
            return Nfa();
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
            return Nfa();
        }
        Nfa A = thompson(sp->lhs);
        Nfa B = thompson(sp->rhs);
        return sp->value == "+" ? concatenate(A, B) : alternate(A, B);
    }
}
