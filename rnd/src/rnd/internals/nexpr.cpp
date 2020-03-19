#include "nexpr.hpp"
#include "zsymbols.hpp"
#include <cassert>

namespace rnd {

// NOTE no need to handle emptyset as special cases to concatenate, closure and
// alternate, because subset construction and minimization handles all cases

void copy_transitions(NExpr& to, const NExpr& from, int offset)
{
    // note that to.symbols and from.symbols might be different
    // also note that to.symbols is always more refined than from.symbols
    ZRange eps;
    for (const auto& [q, dq]: from.states) {
        to.states[q+offset];
        for (const auto& [a, R]: dq) {
            if (a.is_empty()) {
                for (const auto& r: R) {
                    to.states[q+offset][eps].insert(r+offset);
                }
                continue;
            }
            for (const auto& c: to.symbols.cover(a)) {
                assert(!c.is_empty());
                for (const auto& r: R) {
                    to.states[q+offset][c].insert(r+offset);
                }
            }
        }
        if (const auto& deps = dq.find(eps); deps != dq.end()) {
            // don't forget eps transitions
            for (const auto& r: deps->second) {
                to.states[q+offset][eps].insert(r+offset);
            }
        }
    }
}

NExpr closure(const NExpr& A)
{
    NExpr expr;
    expr.symbols = A.symbols;
    int start = 2;
    int accept = start+1;
    copy_transitions(expr, A, start);
    ZRange eps;
    expr.states[0][eps].insert(start);
    expr.states[0][eps].insert(1);
    expr.states[accept][eps].insert(start);
    expr.states[accept][eps].insert(1);
    return expr;
}

NExpr concatenate(const NExpr& A, const NExpr& B)
{
    NExpr expr;
    expr.symbols = A.symbols.combined(B.symbols);
    int as = 2;
    int af = as+1;
    int bs = as + A.states.size();
    int bf = bs+1;
    copy_transitions(expr, A, as);
    copy_transitions(expr, B, bs);
    ZRange eps;
    expr.states[0][eps].insert(as);
    expr.states[af][eps].insert(bs);
    expr.states[bf][eps].insert(1);
    return expr;
}

NExpr alternate(const NExpr& A, const NExpr& B)
{
    NExpr expr;
    expr.symbols = A.symbols.combined(B.symbols);
    int as = 2;
    int af = as+1;
    int bs = as + A.states.size();
    int bf = bs+1;
    copy_transitions(expr, A, as);
    copy_transitions(expr, B, bs);
    ZRange eps;
    expr.states[0][eps].insert(as);
    expr.states[0][eps].insert(bs);
    expr.states[af][eps].insert(1);
    expr.states[bf][eps].insert(1);
    return expr;
}

}
