#include "rnd.h"

struct rnd_expr *empty()
{
    return rnd_expr_symbol(0, 0);
}

struct rnd_expr *optional(struct rnd_expr *a)
{
    return rnd_expr_union(empty(), a);
}

struct rnd_expr *integer()
{
    struct rnd_expr *nonzero = rnd_expr_symbol('1', '9');
    struct rnd_expr *digit = rnd_expr_symbol('0', '9');
    struct rnd_expr *zero = rnd_expr_symbol('0', '0');
    struct rnd_expr *digits = rnd_expr_closure(digit);
    nonzero = rnd_expr_concatenation(nonzero, digits);
    return rnd_expr_union(zero, nonzero);
}

struct rnd_expr *number()
{
    struct rnd_expr *digit = rnd_expr_symbol('0', '9');
    struct rnd_expr *dot = rnd_expr_symbol('.', '.');
    struct rnd_expr *decimal = rnd_expr_concatenation(dot, 
            rnd_expr_concatenation(digit, rnd_expr_closure(digit)));
    struct rnd_expr *eE = rnd_expr_union(rnd_expr_symbol('e', 'e'), rnd_expr_symbol('E', 'E'));
    struct rnd_expr *sign = optional(rnd_expr_union(rnd_expr_symbol('-', '-'), rnd_expr_symbol('+', '+')));
    struct rnd_expr *exponent = rnd_expr_concatenation(eE, rnd_expr_concatenation(sign, integer()));
    return rnd_expr_concatenation(integer(), rnd_expr_concatenation(optional(decimal), optional(exponent)));
}

struct rnd_expr *identifier()
{
    struct rnd_expr *underscore = rnd_expr_symbol('_', '_');
    struct rnd_expr *small = rnd_expr_symbol('a', 'z');
    struct rnd_expr *big = rnd_expr_symbol('A', 'Z');
    struct rnd_expr *alpha = rnd_expr_union(small, big);
    struct rnd_expr *letter = rnd_expr_union(underscore, alpha);
    struct rnd_expr *digit = rnd_expr_symbol('0', '9');
    struct rnd_expr *mid = rnd_expr_closure(rnd_expr_union(letter, digit));
    return rnd_expr_concatenation(letter, mid);
}

struct rnd_expr *whitespace()
{
    struct rnd_expr *space = rnd_expr_symbol(' ', ' ');
    struct rnd_expr *tab = rnd_expr_symbol('\t', '\t');
    struct rnd_expr *newline = rnd_expr_symbol('\n', '\n');
    return rnd_expr_union(space, rnd_expr_union(tab, newline));
}

void test(struct rnd_expr* (*expr_fn)(void))
{
    struct rnd_expr *expr = expr_fn();
    struct rnd_dfa dfa = rnd_convert(expr);
    rnd_expr_destroy(expr);
    rnd_dfa_destroy(&dfa);
}

int main()
{
    test(empty);
    test(integer);
    test(number);
    test(identifier);
    test(whitespace);
}
