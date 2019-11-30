#include "rnd.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#define test(expr_fn) do {\
    struct rnd_expr *expr = expr_fn();\
    struct rnd_dfa dfa = rnd_convert(expr);\
    printf(#expr_fn "\n");\
    print_expr(expr);\
    printf("\n");\
    print_dfa(&dfa);\
    printf("\n\n");\
    rnd_expr_destroy(expr);\
    rnd_dfa_destroy(&dfa);\
} while (0)

void print_symbol_range_c(struct rnd_range range)
{
    if (range.start+1 == range.end) {
        printf("%c", range.start);
    } else if (range.start < range.end) {
        printf("[%c, %c]", range.start, range.end-1);
    }
}

void print_symbol_range_d(struct rnd_range range)
{
    if (range.start+1 == range.end) {
        printf("%d", range.start);
    } else if (range.start < range.end) {
        printf("[%d, %d]", range.start, range.end-1);
    }
}

void print_expr(struct rnd_expr *expr)
{
    switch (expr->type) {
    case RND_SYMBOL:
        print_symbol_range_d(expr->value);
        break;
    case RND_UNION:
        printf("union(");
        print_expr(expr->left);
        printf(", ");
        print_expr(expr->right);
        printf(")");
        break;
    case RND_CONCATENATION:
        printf("concatenation(");
        print_expr(expr->left);
        printf(", ");
        print_expr(expr->right);
        printf(")");
        break;
    case RND_CLOSURE:
        printf("closure(");
        print_expr(expr->left);
        printf(")");
        break;
    default:
        assert(false);
    }
}

void print_dfa(struct rnd_dfa *dfa)
{
    if (!dfa) {
        return;
    }
    printf("start: 0\n");
    printf("error: %d\n", dfa->error);
    printf("accept:");
    for (int i = 0; i < dfa->order; ++i) {
        if (dfa->states[i].accept) {
            printf(" %d", i);
        }
    }
    printf("\ndelta:\n");
    for (int q = 0; q < dfa->order; ++q) {
        for (int i = 0; i < dfa->states[q].outdegree; ++i) {
            struct rnd_transition trans = dfa->states[q].transitions[i];
            printf("d(%d, ", q);
            print_symbol_range_c(trans.label);
            printf(") = %d\n", trans.to);
        }
    }
}

struct rnd_expr *empty()
{
    return rnd_symbol(-1, -1);
}

struct rnd_expr *optional(struct rnd_expr *a)
{
    return rnd_union(empty(), a);
}

struct rnd_expr *integer()
{
    struct rnd_expr *nonzero = rnd_symbol('1', '9'+1);
    struct rnd_expr *digit = rnd_symbol('0', '9'+1);
    struct rnd_expr *zero = rnd_symbol('0', '0'+1);
    struct rnd_expr *digits = rnd_closure(digit);
    nonzero = rnd_concatenation(nonzero, digits);
    return rnd_union(zero, nonzero);
}

struct rnd_expr *number()
{
    struct rnd_expr *digit = rnd_symbol('0', '9'+1);
    struct rnd_expr *dot = rnd_symbol('.', '.'+1);
    struct rnd_expr *decimal = rnd_concatenation(dot, 
            rnd_concatenation(digit, rnd_closure(digit)));
    struct rnd_expr *eE = rnd_union(rnd_symbol('e', 'e'+1), rnd_symbol('E', 'E'+1));
    struct rnd_expr *sign = optional(rnd_union(rnd_symbol('-', '-'+1), rnd_symbol('+', '+'+1)));
    struct rnd_expr *exponent = rnd_concatenation(eE, rnd_concatenation(sign, integer()));
    return rnd_concatenation(integer(), rnd_concatenation(optional(decimal), optional(exponent)));
}

struct rnd_expr *identifier()
{
    struct rnd_expr *underscore = rnd_symbol('_', '_'+1);
    struct rnd_expr *small = rnd_symbol('a', 'z'+1);
    struct rnd_expr *big = rnd_symbol('A', 'Z'+1);
    struct rnd_expr *alpha = rnd_union(small, big);
    struct rnd_expr *letter = rnd_union(underscore, alpha);
    struct rnd_expr *digit = rnd_symbol('0', '9'+1);
    struct rnd_expr *mid = rnd_closure(rnd_union(letter, digit));
    return rnd_concatenation(letter, mid);
}

struct rnd_expr *whitespace()
{
    struct rnd_expr *space = rnd_symbol(' ', ' '+1);
    struct rnd_expr *tab = rnd_symbol('\t', '\t'+1);
    struct rnd_expr *newline = rnd_symbol('\n', '\n'+1);
    return rnd_union(space, rnd_union(tab, newline));
}

struct rnd_expr *character()
{
    struct rnd_expr *backslash = rnd_symbol('\\', '\\'+1);
    struct rnd_expr *quote = rnd_symbol('\'', '\''+1);
    struct rnd_expr *t = rnd_symbol('t', 't'+1);
    struct rnd_expr *n = rnd_symbol('n', 'n'+1);

    struct rnd_expr *escape = rnd_concatenation(backslash, rnd_union(
                quote, rnd_union(backslash, rnd_union(t, n))));
    struct rnd_expr *middle = rnd_union(
            rnd_symbol(32, 38+1),
            rnd_union(
                rnd_symbol(40, 91+1),
                rnd_union(
                    rnd_symbol(93, 126+1),
                    escape)));
    return rnd_concatenation(quote, rnd_concatenation(middle, quote));
}

struct rnd_expr *string()
{
    struct rnd_expr *quotes = rnd_symbol('"', '"'+1);
    struct rnd_expr *backslash = rnd_symbol('\\', '\\'+1);
    struct rnd_expr *middle = rnd_closure(
            rnd_union(
                rnd_symbol(32, 33+1),
                rnd_union(
                    rnd_symbol(35, 91+1),
                    rnd_union(
                        rnd_symbol(93, 126+1),
                        rnd_concatenation(
                            backslash,
                            rnd_symbol(32, 126+1))))));
    return rnd_concatenation(quotes, rnd_concatenation(middle, quotes));
}

int main()
{
    test(empty);
    test(integer);
    test(number);
    test(identifier);
    test(whitespace);
    test(character);
    test(string);
}
