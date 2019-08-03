#include "rnd.h"
#include "regex.h"
#include "dfa.h"
#include <cstdlib>
#include <exception>

int expr_counter = 0;

namespace rnd
{
    struct MalformedExpressionException: public std::exception {};
}

const char* MEMORY_ERROR = "Ran out of memory";
const char* UNEXPECTED_ERROR = "Internal error";
const char* INPUT_ERROR = "Input was malformed";

struct rnd_dfa rnd_dfa_error(struct rnd_dfa* dfa, const char* error_msg)
{
    if (dfa) {
        if (dfa->transitions) {
            free(dfa->transitions);
        }
        if (dfa->accept_states) {
            free(dfa->accept_states);
        }
    }
    return (struct rnd_dfa) {
        .start_state = -1,
        .error = error_msg
    };
}

regex::Expr transform_expr(struct rnd_expr *expr)
{
    if (!expr) {
        return nullptr;
    }

    if (expr->type == RND_SYMBOL) {
        if (expr->left || expr->right) {
            throw rnd::MalformedExpressionException();
        }
        return regex::symbol(expr->symbols.start, expr->symbols.end);
    }

    if (!expr->left) {
        throw rnd::MalformedExpressionException();
    }
    if (expr->type == RND_CLOSURE) {
        auto left = transform_expr(expr->left);
        return regex::closure(left);
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

struct rnd_dfa transform_dfa(const automata::Dfa& dfa)
{
    struct rnd_dfa result = {
        .number_states = dfa.delta.size(),
        .number_accept_states = dfa.accept.size(),
        .start_state = dfa.start,
        .transitions = NULL,
        .error = NULL
    };

    result.accept_states = (int*)malloc(sizeof(int) * result.number_accept_states);
    if (!result.accept_states) {
        return rnd_dfa_error(&result, MEMORY_ERROR);
    }

    size_t i = 0;
    for (int q: dfa.accept) {
        result.accept_states[i++] = q;
    }

    i = 0;
    for (const auto& [q, trans]: dfa.delta) {
        void *temp = realloc(result.transitions, 
                sizeof(struct transition) * (i + trans.size()));
        if (!temp) {
            return rnd_dfa_error(&result, MEMORY_ERROR);
        }
        result.transitions = (struct transition*)temp;
        for (const auto& [a, r]: trans) {
            result.transitions[i] = (struct transition) {
                .current_state = q,
                .next_state = r,
                .symbols = (struct rnd_symbol_interval) {
                    .start = a.start,
                    .end = a.end
                }
            };
            ++i;
        }
    }

    result.number_transitions = i;
    return result;
}

struct rnd_dfa rnd_convert(struct rnd_expr* expr)
{
    try {
        return transform_dfa(automata::Dfa(transform_expr(expr)));
    } catch(rnd::MalformedExpressionException()) {
        return (struct rnd_dfa) {
            .error = INPUT_ERROR
        };
    } catch(...) {
        return (struct rnd_dfa) {
            .error = UNEXPECTED_ERROR
        };
    }
}

void rnd_dfa_destroy(struct rnd_dfa* dfa)
{
    dfa->number_states = 0;
    dfa->number_transitions = 0;
    dfa->number_accept_states = 0;
    dfa->start_state = -1;
    dfa->error = NULL;
    if (dfa->transitions) {
        free(dfa->transitions);
        dfa->transitions = NULL;
    }
    if (dfa->accept_states) {
        free(dfa->accept_states);
        dfa->accept_states = NULL;
    }
}

struct rnd_expr* rnd_expr_new(
        enum rnd_expr_type type, 
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
        expr->symbols.start = start;
        expr->symbols.end = end;
    }
    return expr;
}

struct rnd_expr* rnd_expr_symbol(int start, int end)
{
    return rnd_expr_new(RND_SYMBOL, NULL, NULL, start, end);
}

struct rnd_expr* rnd_expr_union(
        struct rnd_expr* left, 
        struct rnd_expr* right)
{
    return rnd_expr_new(RND_UNION, left, right, 0, 0);
}

struct rnd_expr* rnd_expr_concatenation(
        struct rnd_expr* left,
        struct rnd_expr* right)
{
    return rnd_expr_new(RND_CONCATENATION, left, right, 0, 0);
}

struct rnd_expr* rnd_expr_closure(struct rnd_expr* left)
{
    return rnd_expr_new(RND_CLOSURE, left, NULL, 0, 0);
}

void rnd_expr_destroy(struct rnd_expr* expr)
{
    if (expr) {
        rnd_expr_destroy(expr->left);
        rnd_expr_destroy(expr->right);
        free(expr);
        --expr_counter;
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
