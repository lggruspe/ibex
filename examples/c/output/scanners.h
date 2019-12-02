#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define create_recognizer(...) (struct recognizer) { \
    .token = TOKEN_EMPTY, \
    .accept = false, \
    .error = -1, \
    .transition = NULL, \
    __VA_ARGS__
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
            return { .status = 1, .next_state = 1 };
        if (95 <= a && a < 96)
            return { .status = 1, .next_state = 1 };
        if (97 <= a && a < 123)
            return { .status = 1, .next_state = 1 };
        return { .status = -1, .next_state = 2 };
    case 1:
        if (48 <= a && a < 58)
            return { .status = 1, .next_state = 1 };
        if (65 <= a && a < 91)
            return { .status = 1, .next_state = 1 };
        if (95 <= a && a < 96)
            return { .status = 1, .next_state = 1 };
        if (97 <= a && a < 123)
            return { .status = 1, .next_state = 1 };
        return { .status = -1, .next_state = 2 };
    default:
        return { .status = -1, .next_state = 2 };
    }
}

struct transition_output transition_whitespace(int q, uint32_t a)
{
    switch (q) {
    case 0:
        if (9 <= a && a < 10)
            return { .status = 1, .next_state = 1 };
        if (10 <= a && a < 11)
            return { .status = 1, .next_state = 1 };
        if (32 <= a && a < 33)
            return { .status = 1, .next_state = 1 };
        return { .status = -1, .next_state = 2 };
    case 1:
        return { .status = -1, .next_state = 2 };
    default:
        return { .status = -1, .next_state = 2 };
    }
}

struct transition_output transition_number(int q, uint32_t a)
{
    switch (q) {
    case 0:
        if (48 <= a && a < 49)
            return { .status = 1, .next_state = 3 };
        if (49 <= a && a < 58)
            return { .status = 1, .next_state = 8 };
        return { .status = -1, .next_state = 6 };
    case 3:
        if (46 <= a && a < 47)
            return { .status = 0, .next_state = 7 };
        if (69 <= a && a < 70)
            return { .status = 0, .next_state = 5 };
        if (101 <= a && a < 102)
            return { .status = 0, .next_state = 5 };
        return { .status = -1, .next_state = 6 };
    case 8:
        if (46 <= a && a < 47)
            return { .status = 0, .next_state = 7 };
        if (48 <= a && a < 49)
            return { .status = 1, .next_state = 8 };
        if (49 <= a && a < 58)
            return { .status = 1, .next_state = 8 };
        if (69 <= a && a < 70)
            return { .status = 0, .next_state = 5 };
        if (101 <= a && a < 102)
            return { .status = 0, .next_state = 5 };
        return { .status = -1, .next_state = 6 };
    case 1:
        return { .status = -1, .next_state = 6 };
    case 2:
        if (48 <= a && a < 49)
            return { .status = 1, .next_state = 1 };
        if (49 <= a && a < 58)
            return { .status = 1, .next_state = 4 };
        return { .status = -1, .next_state = 6 };
    case 4:
        if (48 <= a && a < 49)
            return { .status = 1, .next_state = 4 };
        if (49 <= a && a < 58)
            return { .status = 1, .next_state = 4 };
        return { .status = -1, .next_state = 6 };
    case 7:
        if (48 <= a && a < 49)
            return { .status = 1, .next_state = 9 };
        if (49 <= a && a < 58)
            return { .status = 1, .next_state = 9 };
        return { .status = -1, .next_state = 6 };
    case 5:
        if (43 <= a && a < 44)
            return { .status = 0, .next_state = 2 };
        if (45 <= a && a < 46)
            return { .status = 0, .next_state = 2 };
        if (48 <= a && a < 49)
            return { .status = 1, .next_state = 1 };
        if (49 <= a && a < 58)
            return { .status = 1, .next_state = 4 };
        return { .status = -1, .next_state = 6 };
    case 9:
        if (48 <= a && a < 49)
            return { .status = 1, .next_state = 9 };
        if (49 <= a && a < 58)
            return { .status = 1, .next_state = 9 };
        if (69 <= a && a < 70)
            return { .status = 0, .next_state = 5 };
        if (101 <= a && a < 102)
            return { .status = 0, .next_state = 5 };
        return { .status = -1, .next_state = 6 };
    default:
        return { .status = -1, .next_state = 6 };
    }
}

struct transition_output transition_character(int q, uint32_t a)
{
    switch (q) {
    case 0:
        if (39 <= a && a < 40)
            return { .status = 0, .next_state = 3 };
        return { .status = -1, .next_state = 5 };
    case 3:
        if (32 <= a && a < 39)
            return { .status = 0, .next_state = 2 };
        if (40 <= a && a < 92)
            return { .status = 0, .next_state = 2 };
        if (92 <= a && a < 93)
            return { .status = 0, .next_state = 4 };
        if (93 <= a && a < 110)
            return { .status = 0, .next_state = 2 };
        if (110 <= a && a < 111)
            return { .status = 0, .next_state = 2 };
        if (111 <= a && a < 116)
            return { .status = 0, .next_state = 2 };
        if (116 <= a && a < 117)
            return { .status = 0, .next_state = 2 };
        if (117 <= a && a < 127)
            return { .status = 0, .next_state = 2 };
        return { .status = -1, .next_state = 5 };
    case 1:
        return { .status = -1, .next_state = 5 };
    case 2:
        if (39 <= a && a < 40)
            return { .status = 1, .next_state = 1 };
        return { .status = -1, .next_state = 5 };
    case 4:
        if (39 <= a && a < 40)
            return { .status = 0, .next_state = 2 };
        if (92 <= a && a < 93)
            return { .status = 0, .next_state = 2 };
        if (110 <= a && a < 111)
            return { .status = 0, .next_state = 2 };
        if (116 <= a && a < 117)
            return { .status = 0, .next_state = 2 };
        return { .status = -1, .next_state = 5 };
    default:
        return { .status = -1, .next_state = 5 };
    }
}

struct transition_output transition_string(int q, uint32_t a)
{
    switch (q) {
    case 0:
        if (34 <= a && a < 35)
            return { .status = 0, .next_state = 2 };
        return { .status = -1, .next_state = 4 };
    case 2:
        if (32 <= a && a < 34)
            return { .status = 0, .next_state = 2 };
        if (34 <= a && a < 35)
            return { .status = 1, .next_state = 1 };
        if (35 <= a && a < 92)
            return { .status = 0, .next_state = 2 };
        if (92 <= a && a < 93)
            return { .status = 0, .next_state = 3 };
        if (93 <= a && a < 127)
            return { .status = 0, .next_state = 2 };
        return { .status = -1, .next_state = 4 };
    case 1:
        return { .status = -1, .next_state = 4 };
    case 3:
        if (32 <= a && a < 34)
            return { .status = 0, .next_state = 2 };
        if (34 <= a && a < 35)
            return { .status = 0, .next_state = 2 };
        if (35 <= a && a < 92)
            return { .status = 0, .next_state = 2 };
        if (92 <= a && a < 93)
            return { .status = 0, .next_state = 2 };
        if (93 <= a && a < 127)
            return { .status = 0, .next_state = 2 };
        return { .status = -1, .next_state = 4 };
    default:
        return { .status = -1, .next_state = 4 };
    }
}

struct transition_output transition_dot(int q, uint32_t a)
{
    switch (q) {
    case 0:
        if (46 <= a && a < 47)
            return { .status = 1, .next_state = 1 };
        return { .status = -1, .next_state = 2 };
    case 1:
        return { .status = -1, .next_state = 2 };
    default:
        return { .status = -1, .next_state = 2 };
    }
}

struct transition_output transition_lparen(int q, uint32_t a)
{
    switch (q) {
    case 0:
        if (40 <= a && a < 41)
            return { .status = 1, .next_state = 1 };
        return { .status = -1, .next_state = 2 };
    case 1:
        return { .status = -1, .next_state = 2 };
    default:
        return { .status = -1, .next_state = 2 };
    }
}

struct transition_output transition_rparen(int q, uint32_t a)
{
    switch (q) {
    case 0:
        if (41 <= a && a < 42)
            return { .status = 1, .next_state = 1 };
        return { .status = -1, .next_state = 2 };
    case 1:
        return { .status = -1, .next_state = 2 };
    default:
        return { .status = -1, .next_state = 2 };
    }
}

struct transition_output transition_comma(int q, uint32_t a)
{
    switch (q) {
    case 0:
        if (44 <= a && a < 45)
            return { .status = 1, .next_state = 1 };
        return { .status = -1, .next_state = 2 };
    case 1:
        return { .status = -1, .next_state = 2 };
    default:
        return { .status = -1, .next_state = 2 };
    }
}

struct transition_output transition_star(int q, uint32_t a)
{
    switch (q) {
    case 0:
        if (42 <= a && a < 43)
            return { .status = 1, .next_state = 1 };
        return { .status = -1, .next_state = 2 };
    case 1:
        return { .status = -1, .next_state = 2 };
    default:
        return { .status = -1, .next_state = 2 };
    }
}

struct transition_output transition_equal(int q, uint32_t a)
{
    switch (q) {
    case 0:
        if (61 <= a && a < 62)
            return { .status = 1, .next_state = 1 };
        return { .status = -1, .next_state = 2 };
    case 1:
        return { .status = -1, .next_state = 2 };
    default:
        return { .status = -1, .next_state = 2 };
    }
}

struct transition_output transition_lbrace(int q, uint32_t a)
{
    switch (q) {
    case 0:
        if (123 <= a && a < 124)
            return { .status = 1, .next_state = 1 };
        return { .status = -1, .next_state = 2 };
    case 1:
        return { .status = -1, .next_state = 2 };
    default:
        return { .status = -1, .next_state = 2 };
    }
}

struct transition_output transition_rbrace(int q, uint32_t a)
{
    switch (q) {
    case 0:
        if (125 <= a && a < 126)
            return { .status = 1, .next_state = 1 };
        return { .status = -1, .next_state = 2 };
    case 1:
        return { .status = -1, .next_state = 2 };
    default:
        return { .status = -1, .next_state = 2 };
    }
}

struct transition_output transition_colon(int q, uint32_t a)
{
    switch (q) {
    case 0:
        if (58 <= a && a < 59)
            return { .status = 1, .next_state = 1 };
        return { .status = -1, .next_state = 2 };
    case 1:
        return { .status = -1, .next_state = 2 };
    default:
        return { .status = -1, .next_state = 2 };
    }
}

struct transition_output transition_lbracket(int q, uint32_t a)
{
    switch (q) {
    case 0:
        if (91 <= a && a < 92)
            return { .status = 1, .next_state = 1 };
        return { .status = -1, .next_state = 2 };
    case 1:
        return { .status = -1, .next_state = 2 };
    default:
        return { .status = -1, .next_state = 2 };
    }
}

struct transition_output transition_rbracket(int q, uint32_t a)
{
    switch (q) {
    case 0:
        if (93 <= a && a < 94)
            return { .status = 1, .next_state = 1 };
        return { .status = -1, .next_state = 2 };
    case 1:
        return { .status = -1, .next_state = 2 };
    default:
        return { .status = -1, .next_state = 2 };
    }
}

struct transition_output transition_plus(int q, uint32_t a)
{
    switch (q) {
    case 0:
        if (43 <= a && a < 44)
            return { .status = 1, .next_state = 1 };
        return { .status = -1, .next_state = 2 };
    case 1:
        return { .status = -1, .next_state = 2 };
    default:
        return { .status = -1, .next_state = 2 };
    }
}

struct transition_output transition_minus(int q, uint32_t a)
{
    switch (q) {
    case 0:
        if (45 <= a && a < 46)
            return { .status = 1, .next_state = 1 };
        return { .status = -1, .next_state = 2 };
    case 1:
        return { .status = -1, .next_state = 2 };
    default:
        return { .status = -1, .next_state = 2 };
    }
}

struct transition_output transition_slash(int q, uint32_t a)
{
    switch (q) {
    case 0:
        if (47 <= a && a < 48)
            return { .status = 1, .next_state = 1 };
        return { .status = -1, .next_state = 2 };
    case 1:
        return { .status = -1, .next_state = 2 };
    default:
        return { .status = -1, .next_state = 2 };
    }
}

struct transition_output transition_lessthan(int q, uint32_t a)
{
    switch (q) {
    case 0:
        if (60 <= a && a < 61)
            return { .status = 1, .next_state = 1 };
        return { .status = -1, .next_state = 2 };
    case 1:
        return { .status = -1, .next_state = 2 };
    default:
        return { .status = -1, .next_state = 2 };
    }
}

struct transition_output transition_greaterthan(int q, uint32_t a)
{
    switch (q) {
    case 0:
        if (62 <= a && a < 63)
            return { .status = 1, .next_state = 1 };
        return { .status = -1, .next_state = 2 };
    case 1:
        return { .status = -1, .next_state = 2 };
    default:
        return { .status = -1, .next_state = 2 };
    }
}
