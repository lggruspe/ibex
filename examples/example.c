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

void print_symbol_range_c(struct rnd_symbol_interval range)
{
    if (range.start == range.end) {
        printf("%c", range.start);
    } else if (range.start < range.end) {
        printf("[%c, %c]", range.start, range.end);
    }
}

void print_symbol_range_d(struct rnd_symbol_interval range)
{
    if (range.start == range.end) {
        printf("%d", range.start);
    } else if (range.start < range.end) {
        printf("[%d, %d]", range.start, range.end);
    }
}

void print_expr(struct rnd_expr *expr)
{
    switch (expr->type) {
    case RND_SYMBOL:
        print_symbol_range_d(expr->symbols);
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
    printf("start: %d\n", dfa->start_state);
    printf("accept:");
    for (size_t i = 0; i < dfa->number_accept_states; ++i) {
        printf(" %d", dfa->accept_states[i]);
    }
    printf("\ndelta:\n");
    for (size_t i = 0; i < dfa->number_transitions; ++i) {
        struct rnd_transition trans = dfa->transitions[i];
        printf("d(%d, ", trans.current_state);
        print_symbol_range_c(trans.symbols);
        printf(") = %d\n", trans.next_state);
    }
}

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

struct rnd_expr *character()
{
    struct rnd_expr *backslash = rnd_expr_symbol('\\', '\\');
    struct rnd_expr *quote = rnd_expr_symbol('\'', '\'');
    struct rnd_expr *t = rnd_expr_symbol('t', 't');
    struct rnd_expr *n = rnd_expr_symbol('n', 'n');

    struct rnd_expr *escape = rnd_expr_concatenation(backslash, rnd_expr_union(
                quote, rnd_expr_union(backslash, rnd_expr_union(t, n))));
    struct rnd_expr *middle = rnd_expr_union(
            rnd_expr_symbol(32, 38),
            rnd_expr_union(
                rnd_expr_symbol(40, 91),
                rnd_expr_union(
                    rnd_expr_symbol(93, 126),
                    escape)));
    return rnd_expr_concatenation(quote, rnd_expr_concatenation(middle, quote));
}

struct rnd_expr *string()
{
    struct rnd_expr *quotes = rnd_expr_symbol('"', '"');
    struct rnd_expr *backslash = rnd_expr_symbol('\\', '\\');
    struct rnd_expr *middle = rnd_expr_closure(
            rnd_expr_union(
                rnd_expr_symbol(32, 33),
                rnd_expr_union(
                    rnd_expr_symbol(35, 91),
                    rnd_expr_union(
                        rnd_expr_symbol(93, 126),
                        rnd_expr_concatenation(
                            backslash,
                            rnd_expr_symbol(32, 126))))));
    return rnd_expr_concatenation(quotes, rnd_expr_concatenation(middle, quotes));
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
