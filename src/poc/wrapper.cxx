#include "rnd.h"
#include "automaton.hpp"
#include "nexpr.hpp"
#include "zsymbols.hpp"
#include <cassert>
#include <unordered_set>
#include <vector>

NExpr ir_to_expr(rnd_expr* c_expr)
{
    // c_expr must not be null
    switch (c_expr->type) {
    case RND_SYMBOL: {
        ZRange in(c_expr->value.start, c_expr->value.end);
        return NExpr(in);
    }
    case RND_UNION:
        return alternate(ir_to_expr(c_expr->left), ir_to_expr(c_expr->right));
    case RND_CONCATENATION:
        return concatenate(ir_to_expr(c_expr->left), ir_to_expr(c_expr->right));
    case RND_CLOSURE:
        return closure(ir_to_expr(c_expr->left));
    default:
        assert(false);
    }
    return NExpr();
}

rnd_dfa serialize(const Automaton& fsm)
{
    rnd_dfa dfa;
    dfa.order = fsm.states.size();
    dfa.states = new rnd_state[dfa.order];
    dfa.error = fsm.error;
    int i = 0;
    for (const auto& [q, dq]: fsm.states) {
        assert(i == q);
        auto& state = dfa.states[q];
        state.accept = false;
        state.outdegree = dq.size();
        state.transitions = new rnd_transition[state.outdegree];
        auto& trans = state.transitions;
        int j = 0;
        for (const auto& [a, r]: dq) {
            trans[j].label.start = a.start;
            trans[j].label.end = a.end;
            trans[j].to = r;
            ++j;
        }
        ++i;
    }
    for (const auto& q: fsm.accepts) {
        dfa.states[q].accept = true;
    }
    return dfa;
}

rnd_dfa rnd_convert(rnd_expr* c_expr)
{
    Automaton m(ir_to_expr(c_expr));
    return serialize(m);
}

void rnd_dfa_destroy(rnd_dfa* dfa)
{
    // doesn't free dfa
    if (dfa) {
        if (dfa->states) {
            for (int i = 0; i < dfa->order; ++i) {
                delete[] dfa->states[i].transitions;
            }
            delete[] dfa->states;
            dfa->states = nullptr;
        }
        dfa->order = 0;
    }
}

int expr_counter;

rnd_expr* rnd_expr_new(
    rnd_type type,
    rnd_expr* left = nullptr,
    rnd_expr* right = nullptr,
    unsigned int start = 0,
    unsigned int end = 0)
{
    rnd_expr* expr = new rnd_expr;
    ++expr_counter;
    expr->type = type;
    expr->left = left;
    expr->right = right;
    expr->value.start = start;
    expr->value.end = end;
    return expr;
}

rnd_expr* rnd_symbol(unsigned int start, unsigned int end)
{
    return rnd_expr_new(RND_SYMBOL, nullptr, nullptr, start, end); 
}

rnd_expr* rnd_union(rnd_expr* left, rnd_expr* right)
{
    return rnd_expr_new(RND_UNION, left, right);
}

rnd_expr* rnd_concatenation(rnd_expr* left, rnd_expr* right)
{
    return rnd_expr_new(RND_CONCATENATION, left, right);
}

rnd_expr* rnd_closure(rnd_expr* left)
{
    return rnd_expr_new(RND_CLOSURE, left);
}

void rnd_expr_destroy(rnd_expr* expr)
{
    if (!expr) {
        return;
    }
    std::vector<rnd_expr*> stack = {expr};
    std::unordered_set<rnd_expr*> freed = {nullptr};
    while (!stack.empty()) {
        auto re = stack.back();
        stack.pop_back();
        if (freed.find(re) != freed.end()) {
            continue;
        }
        stack.push_back(re->left);
        stack.push_back(re->right);
        rnd_expr_free(re);
        freed.insert(re);
    }
}

void rnd_expr_free(rnd_expr* expr)
{
    if (expr) {
        delete expr;
        --expr_counter;
    }
}

int rnd_get_expr_counter()
{
    return expr_counter;
}
