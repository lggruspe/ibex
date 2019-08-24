#include "rnd.h"
#include "rnd/dfa/dfa.h"
#include <cassert>
#include <cstdlib>
#include <exception>
#include <unordered_set>
#include <vector>

int expr_counter = 0;

namespace rnd
{
    struct MalformedExpressionException: public std::exception {};
}

const char* MEMORY_ERROR = "Ran out of memory";
const char* UNEXPECTED_ERROR = "Internal error";
const char* INPUT_ERROR = "Input was malformed";

rnd::regex::Expr transform_expr(struct rnd_expr *expr)
{
    if (!expr) {
        return nullptr;
    }

    if (expr->type == RND_SYMBOL) {
        if (expr->left || expr->right) {
            throw rnd::MalformedExpressionException();
        }
        return rnd::regex::symbol(expr->value.start, expr->value.end);
    }

    if (!expr->left) {
        throw rnd::MalformedExpressionException();
    }
    if (expr->type == RND_CLOSURE) {
        auto left = transform_expr(expr->left);
        return rnd::regex::closure(left);
    }

    if (!expr->right) {
        throw rnd::MalformedExpressionException();
    }
    if (expr->type == RND_UNION) {
        auto left = transform_expr(expr->left);
        auto right = transform_expr(expr->right);
        return left | right;
    }
    if (expr->type == RND_CONCATENATION) {
        auto left = transform_expr(expr->left);
        auto right = transform_expr(expr->right);
        return left + right;
    }
    throw rnd::MalformedExpressionException();
}

struct rnd_dfa rnd_dfa_error(struct rnd_dfa* dfa, const char* error_msg)
{
    rnd_dfa_destroy(dfa);
    dfa->error = error_msg;
    return *dfa;
}

struct rnd_dfa transform_dfa(const rnd::dfa::Dfa& dfa)
{
    struct rnd_dfa result;
    result.states = NULL;
    result.error = NULL;
    result.order = dfa.delta.size();
    if (result.order) {
        result.states = (struct rnd_state*)malloc(sizeof(struct rnd_state) * result.order);
        if (!result.states) {
            return rnd_dfa_error(&result, MEMORY_ERROR);
        }
    }

    for (const auto& [q, trans]: dfa.delta) {
        assert(0 <= q && q < result.order);
        result.states[q].accept = false;
        result.states[q].outdegree = trans.size();
        result.states[q].transitions = NULL;
        if (result.states[q].outdegree) {
            void* temp = malloc(sizeof(struct rnd_transition) * result.states[q].outdegree);
            result.states[q].transitions = (struct rnd_transition*)temp;
            if (!result.states[q].transitions) {
                return rnd_dfa_error(&result, MEMORY_ERROR);
            }

            int i = 0;
            for (const auto& [a, r]: trans) {
                result.states[q].transitions[i].label.start = a.start;
                result.states[q].transitions[i].label.end = a.end;
                result.states[q].transitions[i].to = r;
                ++i;
            }
        }
    }

    for (const auto& q: dfa.accept) {
        assert(0 <= q && q < result.order);
        result.states[q].accept = true;
    }
    return result;
}

struct rnd_dfa rnd_convert(struct rnd_expr* expr)
{
    try {
        return transform_dfa(rnd::dfa::Dfa(transform_expr(expr)));
    } catch(rnd::MalformedExpressionException()) {
        struct rnd_dfa dfa;
        rnd_dfa_error(&dfa, INPUT_ERROR);
        return dfa;
    } catch(...) {
        struct rnd_dfa dfa;
        rnd_dfa_error(&dfa, UNEXPECTED_ERROR);
        return dfa;
    }
}

void rnd_dfa_destroy(struct rnd_dfa* dfa)
{
    if (dfa) {
        if (dfa->states) {
            for (int i = 0; i < dfa->order; ++i) {
                if (dfa->states[i].transitions) {
                    free(dfa->states[i].transitions);
                }
            }
            free(dfa->states);
            dfa->states = NULL;
        }
        dfa->order = 0;
        dfa->error = NULL;
    }
}

struct rnd_expr* rnd_expr_new(
        enum rnd_type type, 
        struct rnd_expr* left,
        struct rnd_expr* right,
        int start,
        int end)
{
    struct rnd_expr* expr = (struct rnd_expr*)malloc(sizeof(struct rnd_expr));
    if (expr) {
        ++expr_counter;
        expr->type = type;
        expr->left = left;
        expr->right = right;
        expr->value.start = start;
        expr->value.end = end;
    }
    return expr;
}

struct rnd_expr* rnd_symbol(int start, int end)
{
    return rnd_expr_new(RND_SYMBOL, NULL, NULL, start, end);
}

struct rnd_expr* rnd_union(
        struct rnd_expr* left, 
        struct rnd_expr* right)
{
    return rnd_expr_new(RND_UNION, left, right, 0, 0);
}

struct rnd_expr* rnd_concatenation(
        struct rnd_expr* left,
        struct rnd_expr* right)
{
    return rnd_expr_new(RND_CONCATENATION, left, right, 0, 0);
}

struct rnd_expr* rnd_closure(struct rnd_expr* left)
{
    return rnd_expr_new(RND_CLOSURE, left, NULL, 0, 0);
}

void rnd_expr_destroy(struct rnd_expr* expr)
{
    if (!expr) {
        return;
    }
    std::vector<struct rnd_expr*> stack = { expr };
    std::unordered_set<struct rnd_expr*> freed = { nullptr };
    while (!stack.empty()) {
        auto re = stack.back();
        stack.pop_back();
        if (!re || freed.find(re) != freed.end()) {
            continue;
        }
        stack.push_back(re->left);
        stack.push_back(re->right);
        rnd_expr_free(re);
        freed.insert(re);
    }
}

void rnd_expr_free(struct rnd_expr* expr)
{
    if (expr) {
        free(expr);
        --expr_counter;
    }
}

int rnd_get_expr_counter()
{
    return expr_counter;
}
