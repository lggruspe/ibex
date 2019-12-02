#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define create_recognizer(...) (struct recognizer) { \
    .token = TOKEN_EMPTY, \
    .accept = false, \
    .error = -1, \
    .transition = NULL, \
    __VA_ARGS__ \
}

enum token {
    TOKEN_EMPTY = 0,
    TOKEN_IDENTIFIER,
    TOKEN_WHITESPACE,
    TOKEN_NUMBER,
    TOKEN_CHARACTER,
    TOKEN_STRING,
    TOKEN_DOT,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_COMMA,
    TOKEN_STAR,
    TOKEN_EQUAL,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_COLON,
    TOKEN_LBRACKET,
    TOKEN_RBRACKET,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_SLASH,
    TOKEN_LESSTHAN,
    TOKEN_GREATERTHAN,
};

int print_token(FILE *fp, enum token token)
{
    switch (token) {
    case TOKEN_EMPTY:
        return fprintf(fp, "empty");
    case TOKEN_IDENTIFIER:
        return fprintf(fp, "identifier");
    case TOKEN_WHITESPACE:
        return fprintf(fp, "whitespace");
    case TOKEN_NUMBER:
        return fprintf(fp, "number");
    case TOKEN_CHARACTER:
        return fprintf(fp, "character");
    case TOKEN_STRING:
        return fprintf(fp, "string");
    case TOKEN_DOT:
        return fprintf(fp, "dot");
    case TOKEN_LPAREN:
        return fprintf(fp, "lparen");
    case TOKEN_RPAREN:
        return fprintf(fp, "rparen");
    case TOKEN_COMMA:
        return fprintf(fp, "comma");
    case TOKEN_STAR:
        return fprintf(fp, "star");
    case TOKEN_EQUAL:
        return fprintf(fp, "equal");
    case TOKEN_LBRACE:
        return fprintf(fp, "lbrace");
    case TOKEN_RBRACE:
        return fprintf(fp, "rbrace");
    case TOKEN_COLON:
        return fprintf(fp, "colon");
    case TOKEN_LBRACKET:
        return fprintf(fp, "lbracket");
    case TOKEN_RBRACKET:
        return fprintf(fp, "rbracket");
    case TOKEN_PLUS:
        return fprintf(fp, "plus");
    case TOKEN_MINUS:
        return fprintf(fp, "minus");
    case TOKEN_SLASH:
        return fprintf(fp, "slash");
    case TOKEN_LESSTHAN:
        return fprintf(fp, "lessthan");
    case TOKEN_GREATERTHAN:
        return fprintf(fp, "greaterthan");
    default:
        return -1;
    }
}

struct transition_output {
    int status;
    int next_state;
};

struct recognizer {
    enum token token;
    bool accept;
    int error;
    struct transition_output (*transition)(int, uint32_t);
};

struct transition_output transition_identifier(int q, uint32_t a)
{
    switch (q) {
    case 0:
        if (65 <= a && a < 91)
            return (struct transition_output){ .status = 1, .next_state = 1 };
        if (a == 95)
            return (struct transition_output){ .status = 1, .next_state = 1 };
        if (97 <= a && a < 123)
            return (struct transition_output){ .status = 1, .next_state = 1 };
        return (struct transition_output){ .status = -1, .next_state = 2 };
    case 1:
        if (48 <= a && a < 58)
            return (struct transition_output){ .status = 1, .next_state = 1 };
        if (65 <= a && a < 91)
            return (struct transition_output){ .status = 1, .next_state = 1 };
        if (a == 95)
            return (struct transition_output){ .status = 1, .next_state = 1 };
        if (97 <= a && a < 123)
            return (struct transition_output){ .status = 1, .next_state = 1 };
        return (struct transition_output){ .status = -1, .next_state = 2 };
    default:
        return (struct transition_output){ .status = -1, .next_state = 2 };
    }
}

struct transition_output transition_whitespace(int q, uint32_t a)
{
    switch (q) {
    case 0:
        if (a == 9)
            return (struct transition_output){ .status = 1, .next_state = 1 };
        if (a == 10)
            return (struct transition_output){ .status = 1, .next_state = 1 };
        if (a == 32)
            return (struct transition_output){ .status = 1, .next_state = 1 };
        return (struct transition_output){ .status = -1, .next_state = 2 };
    case 1:
        return (struct transition_output){ .status = -1, .next_state = 2 };
    default:
        return (struct transition_output){ .status = -1, .next_state = 2 };
    }
}

struct transition_output transition_number(int q, uint32_t a)
{
    switch (q) {
    case 0:
        if (a == 48)
            return (struct transition_output){ .status = 1, .next_state = 3 };
        if (49 <= a && a < 58)
            return (struct transition_output){ .status = 1, .next_state = 8 };
        return (struct transition_output){ .status = -1, .next_state = 6 };
    case 3:
        if (a == 46)
            return (struct transition_output){ .status = 0, .next_state = 7 };
        if (a == 69)
            return (struct transition_output){ .status = 0, .next_state = 5 };
        if (a == 101)
            return (struct transition_output){ .status = 0, .next_state = 5 };
        return (struct transition_output){ .status = -1, .next_state = 6 };
    case 8:
        if (a == 46)
            return (struct transition_output){ .status = 0, .next_state = 7 };
        if (a == 48)
            return (struct transition_output){ .status = 1, .next_state = 8 };
        if (49 <= a && a < 58)
            return (struct transition_output){ .status = 1, .next_state = 8 };
        if (a == 69)
            return (struct transition_output){ .status = 0, .next_state = 5 };
        if (a == 101)
            return (struct transition_output){ .status = 0, .next_state = 5 };
        return (struct transition_output){ .status = -1, .next_state = 6 };
    case 1:
        return (struct transition_output){ .status = -1, .next_state = 6 };
    case 2:
        if (a == 48)
            return (struct transition_output){ .status = 1, .next_state = 1 };
        if (49 <= a && a < 58)
            return (struct transition_output){ .status = 1, .next_state = 4 };
        return (struct transition_output){ .status = -1, .next_state = 6 };
    case 4:
        if (a == 48)
            return (struct transition_output){ .status = 1, .next_state = 4 };
        if (49 <= a && a < 58)
            return (struct transition_output){ .status = 1, .next_state = 4 };
        return (struct transition_output){ .status = -1, .next_state = 6 };
    case 7:
        if (a == 48)
            return (struct transition_output){ .status = 1, .next_state = 9 };
        if (49 <= a && a < 58)
            return (struct transition_output){ .status = 1, .next_state = 9 };
        return (struct transition_output){ .status = -1, .next_state = 6 };
    case 5:
        if (a == 43)
            return (struct transition_output){ .status = 0, .next_state = 2 };
        if (a == 45)
            return (struct transition_output){ .status = 0, .next_state = 2 };
        if (a == 48)
            return (struct transition_output){ .status = 1, .next_state = 1 };
        if (49 <= a && a < 58)
            return (struct transition_output){ .status = 1, .next_state = 4 };
        return (struct transition_output){ .status = -1, .next_state = 6 };
    case 9:
        if (a == 48)
            return (struct transition_output){ .status = 1, .next_state = 9 };
        if (49 <= a && a < 58)
            return (struct transition_output){ .status = 1, .next_state = 9 };
        if (a == 69)
            return (struct transition_output){ .status = 0, .next_state = 5 };
        if (a == 101)
            return (struct transition_output){ .status = 0, .next_state = 5 };
        return (struct transition_output){ .status = -1, .next_state = 6 };
    default:
        return (struct transition_output){ .status = -1, .next_state = 6 };
    }
}

struct transition_output transition_character(int q, uint32_t a)
{
    switch (q) {
    case 0:
        if (a == 39)
            return (struct transition_output){ .status = 0, .next_state = 3 };
        return (struct transition_output){ .status = -1, .next_state = 5 };
    case 3:
        if (32 <= a && a < 39)
            return (struct transition_output){ .status = 0, .next_state = 2 };
        if (40 <= a && a < 92)
            return (struct transition_output){ .status = 0, .next_state = 2 };
        if (a == 92)
            return (struct transition_output){ .status = 0, .next_state = 4 };
        if (93 <= a && a < 110)
            return (struct transition_output){ .status = 0, .next_state = 2 };
        if (a == 110)
            return (struct transition_output){ .status = 0, .next_state = 2 };
        if (111 <= a && a < 116)
            return (struct transition_output){ .status = 0, .next_state = 2 };
        if (a == 116)
            return (struct transition_output){ .status = 0, .next_state = 2 };
        if (117 <= a && a < 127)
            return (struct transition_output){ .status = 0, .next_state = 2 };
        return (struct transition_output){ .status = -1, .next_state = 5 };
    case 1:
        return (struct transition_output){ .status = -1, .next_state = 5 };
    case 2:
        if (a == 39)
            return (struct transition_output){ .status = 1, .next_state = 1 };
        return (struct transition_output){ .status = -1, .next_state = 5 };
    case 4:
        if (a == 39)
            return (struct transition_output){ .status = 0, .next_state = 2 };
        if (a == 92)
            return (struct transition_output){ .status = 0, .next_state = 2 };
        if (a == 110)
            return (struct transition_output){ .status = 0, .next_state = 2 };
        if (a == 116)
            return (struct transition_output){ .status = 0, .next_state = 2 };
        return (struct transition_output){ .status = -1, .next_state = 5 };
    default:
        return (struct transition_output){ .status = -1, .next_state = 5 };
    }
}

struct transition_output transition_string(int q, uint32_t a)
{
    switch (q) {
    case 0:
        if (a == 34)
            return (struct transition_output){ .status = 0, .next_state = 2 };
        return (struct transition_output){ .status = -1, .next_state = 4 };
    case 2:
        if (32 <= a && a < 34)
            return (struct transition_output){ .status = 0, .next_state = 2 };
        if (a == 34)
            return (struct transition_output){ .status = 1, .next_state = 1 };
        if (35 <= a && a < 92)
            return (struct transition_output){ .status = 0, .next_state = 2 };
        if (a == 92)
            return (struct transition_output){ .status = 0, .next_state = 3 };
        if (93 <= a && a < 127)
            return (struct transition_output){ .status = 0, .next_state = 2 };
        return (struct transition_output){ .status = -1, .next_state = 4 };
    case 1:
        return (struct transition_output){ .status = -1, .next_state = 4 };
    case 3:
        if (32 <= a && a < 34)
            return (struct transition_output){ .status = 0, .next_state = 2 };
        if (a == 34)
            return (struct transition_output){ .status = 0, .next_state = 2 };
        if (35 <= a && a < 92)
            return (struct transition_output){ .status = 0, .next_state = 2 };
        if (a == 92)
            return (struct transition_output){ .status = 0, .next_state = 2 };
        if (93 <= a && a < 127)
            return (struct transition_output){ .status = 0, .next_state = 2 };
        return (struct transition_output){ .status = -1, .next_state = 4 };
    default:
        return (struct transition_output){ .status = -1, .next_state = 4 };
    }
}

struct transition_output transition_dot(int q, uint32_t a)
{
    switch (q) {
    case 0:
        if (a == 46)
            return (struct transition_output){ .status = 1, .next_state = 1 };
        return (struct transition_output){ .status = -1, .next_state = 2 };
    case 1:
        return (struct transition_output){ .status = -1, .next_state = 2 };
    default:
        return (struct transition_output){ .status = -1, .next_state = 2 };
    }
}

struct transition_output transition_lparen(int q, uint32_t a)
{
    switch (q) {
    case 0:
        if (a == 40)
            return (struct transition_output){ .status = 1, .next_state = 1 };
        return (struct transition_output){ .status = -1, .next_state = 2 };
    case 1:
        return (struct transition_output){ .status = -1, .next_state = 2 };
    default:
        return (struct transition_output){ .status = -1, .next_state = 2 };
    }
}

struct transition_output transition_rparen(int q, uint32_t a)
{
    switch (q) {
    case 0:
        if (a == 41)
            return (struct transition_output){ .status = 1, .next_state = 1 };
        return (struct transition_output){ .status = -1, .next_state = 2 };
    case 1:
        return (struct transition_output){ .status = -1, .next_state = 2 };
    default:
        return (struct transition_output){ .status = -1, .next_state = 2 };
    }
}

struct transition_output transition_comma(int q, uint32_t a)
{
    switch (q) {
    case 0:
        if (a == 44)
            return (struct transition_output){ .status = 1, .next_state = 1 };
        return (struct transition_output){ .status = -1, .next_state = 2 };
    case 1:
        return (struct transition_output){ .status = -1, .next_state = 2 };
    default:
        return (struct transition_output){ .status = -1, .next_state = 2 };
    }
}

struct transition_output transition_star(int q, uint32_t a)
{
    switch (q) {
    case 0:
        if (a == 42)
            return (struct transition_output){ .status = 1, .next_state = 1 };
        return (struct transition_output){ .status = -1, .next_state = 2 };
    case 1:
        return (struct transition_output){ .status = -1, .next_state = 2 };
    default:
        return (struct transition_output){ .status = -1, .next_state = 2 };
    }
}

struct transition_output transition_equal(int q, uint32_t a)
{
    switch (q) {
    case 0:
        if (a == 61)
            return (struct transition_output){ .status = 1, .next_state = 1 };
        return (struct transition_output){ .status = -1, .next_state = 2 };
    case 1:
        return (struct transition_output){ .status = -1, .next_state = 2 };
    default:
        return (struct transition_output){ .status = -1, .next_state = 2 };
    }
}

struct transition_output transition_lbrace(int q, uint32_t a)
{
    switch (q) {
    case 0:
        if (a == 123)
            return (struct transition_output){ .status = 1, .next_state = 1 };
        return (struct transition_output){ .status = -1, .next_state = 2 };
    case 1:
        return (struct transition_output){ .status = -1, .next_state = 2 };
    default:
        return (struct transition_output){ .status = -1, .next_state = 2 };
    }
}

struct transition_output transition_rbrace(int q, uint32_t a)
{
    switch (q) {
    case 0:
        if (a == 125)
            return (struct transition_output){ .status = 1, .next_state = 1 };
        return (struct transition_output){ .status = -1, .next_state = 2 };
    case 1:
        return (struct transition_output){ .status = -1, .next_state = 2 };
    default:
        return (struct transition_output){ .status = -1, .next_state = 2 };
    }
}

struct transition_output transition_colon(int q, uint32_t a)
{
    switch (q) {
    case 0:
        if (a == 58)
            return (struct transition_output){ .status = 1, .next_state = 1 };
        return (struct transition_output){ .status = -1, .next_state = 2 };
    case 1:
        return (struct transition_output){ .status = -1, .next_state = 2 };
    default:
        return (struct transition_output){ .status = -1, .next_state = 2 };
    }
}

struct transition_output transition_lbracket(int q, uint32_t a)
{
    switch (q) {
    case 0:
        if (a == 91)
            return (struct transition_output){ .status = 1, .next_state = 1 };
        return (struct transition_output){ .status = -1, .next_state = 2 };
    case 1:
        return (struct transition_output){ .status = -1, .next_state = 2 };
    default:
        return (struct transition_output){ .status = -1, .next_state = 2 };
    }
}

struct transition_output transition_rbracket(int q, uint32_t a)
{
    switch (q) {
    case 0:
        if (a == 93)
            return (struct transition_output){ .status = 1, .next_state = 1 };
        return (struct transition_output){ .status = -1, .next_state = 2 };
    case 1:
        return (struct transition_output){ .status = -1, .next_state = 2 };
    default:
        return (struct transition_output){ .status = -1, .next_state = 2 };
    }
}

struct transition_output transition_plus(int q, uint32_t a)
{
    switch (q) {
    case 0:
        if (a == 43)
            return (struct transition_output){ .status = 1, .next_state = 1 };
        return (struct transition_output){ .status = -1, .next_state = 2 };
    case 1:
        return (struct transition_output){ .status = -1, .next_state = 2 };
    default:
        return (struct transition_output){ .status = -1, .next_state = 2 };
    }
}

struct transition_output transition_minus(int q, uint32_t a)
{
    switch (q) {
    case 0:
        if (a == 45)
            return (struct transition_output){ .status = 1, .next_state = 1 };
        return (struct transition_output){ .status = -1, .next_state = 2 };
    case 1:
        return (struct transition_output){ .status = -1, .next_state = 2 };
    default:
        return (struct transition_output){ .status = -1, .next_state = 2 };
    }
}

struct transition_output transition_slash(int q, uint32_t a)
{
    switch (q) {
    case 0:
        if (a == 47)
            return (struct transition_output){ .status = 1, .next_state = 1 };
        return (struct transition_output){ .status = -1, .next_state = 2 };
    case 1:
        return (struct transition_output){ .status = -1, .next_state = 2 };
    default:
        return (struct transition_output){ .status = -1, .next_state = 2 };
    }
}

struct transition_output transition_lessthan(int q, uint32_t a)
{
    switch (q) {
    case 0:
        if (a == 60)
            return (struct transition_output){ .status = 1, .next_state = 1 };
        return (struct transition_output){ .status = -1, .next_state = 2 };
    case 1:
        return (struct transition_output){ .status = -1, .next_state = 2 };
    default:
        return (struct transition_output){ .status = -1, .next_state = 2 };
    }
}

struct transition_output transition_greaterthan(int q, uint32_t a)
{
    switch (q) {
    case 0:
        if (a == 62)
            return (struct transition_output){ .status = 1, .next_state = 1 };
        return (struct transition_output){ .status = -1, .next_state = 2 };
    case 1:
        return (struct transition_output){ .status = -1, .next_state = 2 };
    default:
        return (struct transition_output){ .status = -1, .next_state = 2 };
    }
}
