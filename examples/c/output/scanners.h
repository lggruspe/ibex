#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define fail_if_empty(input) do {\
    int c = (input);\
    if (c == 0 || c == EOF) return -1;\
} while (0)

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


int transition_identifier(int state, int input)
{
    fail_if_empty(input);
    switch (state) {
    case 0:
        if (0 <= input && input <= 48)
            return 2;
        if (48 <= input && input <= 58)
            return 2;
        if (58 <= input && input <= 65)
            return 2;
        if (65 <= input && input <= 91)
            return 1;
        if (91 <= input && input <= 95)
            return 2;
        if (95 <= input && input <= 96)
            return 1;
        if (96 <= input && input <= 97)
            return 2;
        if (97 <= input && input <= 123)
            return 1;
        if (123 <= input && input <= 4294967295)
            return 2;
        return -1;
    case 2:
        if (0 <= input && input <= 48)
            return 2;
        if (48 <= input && input <= 58)
            return 2;
        if (58 <= input && input <= 65)
            return 2;
        if (65 <= input && input <= 91)
            return 2;
        if (91 <= input && input <= 95)
            return 2;
        if (95 <= input && input <= 96)
            return 2;
        if (96 <= input && input <= 97)
            return 2;
        if (97 <= input && input <= 123)
            return 2;
        if (123 <= input && input <= 4294967295)
            return 2;
        return -1;
    case 1:
        if (0 <= input && input <= 48)
            return 2;
        if (48 <= input && input <= 58)
            return 1;
        if (58 <= input && input <= 65)
            return 2;
        if (65 <= input && input <= 91)
            return 1;
        if (91 <= input && input <= 95)
            return 2;
        if (95 <= input && input <= 96)
            return 1;
        if (96 <= input && input <= 97)
            return 2;
        if (97 <= input && input <= 123)
            return 1;
        if (123 <= input && input <= 4294967295)
            return 2;
        return -1;
    default:
        return -1;
    }
}

int transition_whitespace(int state, int input)
{
    fail_if_empty(input);
    switch (state) {
    case 0:
        if (0 <= input && input <= 9)
            return 2;
        if (9 <= input && input <= 10)
            return 1;
        if (10 <= input && input <= 11)
            return 1;
        if (11 <= input && input <= 32)
            return 2;
        if (32 <= input && input <= 33)
            return 1;
        if (33 <= input && input <= 4294967295)
            return 2;
        return -1;
    case 2:
        if (0 <= input && input <= 9)
            return 2;
        if (9 <= input && input <= 10)
            return 2;
        if (10 <= input && input <= 11)
            return 2;
        if (11 <= input && input <= 32)
            return 2;
        if (32 <= input && input <= 33)
            return 2;
        if (33 <= input && input <= 4294967295)
            return 2;
        return -1;
    case 1:
        if (0 <= input && input <= 9)
            return 2;
        if (9 <= input && input <= 10)
            return 2;
        if (10 <= input && input <= 11)
            return 2;
        if (11 <= input && input <= 32)
            return 2;
        if (32 <= input && input <= 33)
            return 2;
        if (33 <= input && input <= 4294967295)
            return 2;
        return -1;
    default:
        return -1;
    }
}

int transition_number(int state, int input)
{
    fail_if_empty(input);
    switch (state) {
    case 0:
        if (0 <= input && input <= 43)
            return 6;
        if (43 <= input && input <= 44)
            return 6;
        if (44 <= input && input <= 45)
            return 6;
        if (45 <= input && input <= 46)
            return 6;
        if (46 <= input && input <= 47)
            return 6;
        if (47 <= input && input <= 48)
            return 6;
        if (48 <= input && input <= 49)
            return 3;
        if (49 <= input && input <= 58)
            return 8;
        if (58 <= input && input <= 69)
            return 6;
        if (69 <= input && input <= 70)
            return 6;
        if (70 <= input && input <= 101)
            return 6;
        if (101 <= input && input <= 102)
            return 6;
        if (102 <= input && input <= 4294967295)
            return 6;
        return -1;
    case 6:
        if (0 <= input && input <= 43)
            return 6;
        if (43 <= input && input <= 44)
            return 6;
        if (44 <= input && input <= 45)
            return 6;
        if (45 <= input && input <= 46)
            return 6;
        if (46 <= input && input <= 47)
            return 6;
        if (47 <= input && input <= 48)
            return 6;
        if (48 <= input && input <= 49)
            return 6;
        if (49 <= input && input <= 58)
            return 6;
        if (58 <= input && input <= 69)
            return 6;
        if (69 <= input && input <= 70)
            return 6;
        if (70 <= input && input <= 101)
            return 6;
        if (101 <= input && input <= 102)
            return 6;
        if (102 <= input && input <= 4294967295)
            return 6;
        return -1;
    case 3:
        if (0 <= input && input <= 43)
            return 6;
        if (43 <= input && input <= 44)
            return 6;
        if (44 <= input && input <= 45)
            return 6;
        if (45 <= input && input <= 46)
            return 6;
        if (46 <= input && input <= 47)
            return 7;
        if (47 <= input && input <= 48)
            return 6;
        if (48 <= input && input <= 49)
            return 6;
        if (49 <= input && input <= 58)
            return 6;
        if (58 <= input && input <= 69)
            return 6;
        if (69 <= input && input <= 70)
            return 5;
        if (70 <= input && input <= 101)
            return 6;
        if (101 <= input && input <= 102)
            return 5;
        if (102 <= input && input <= 4294967295)
            return 6;
        return -1;
    case 8:
        if (0 <= input && input <= 43)
            return 6;
        if (43 <= input && input <= 44)
            return 6;
        if (44 <= input && input <= 45)
            return 6;
        if (45 <= input && input <= 46)
            return 6;
        if (46 <= input && input <= 47)
            return 7;
        if (47 <= input && input <= 48)
            return 6;
        if (48 <= input && input <= 49)
            return 8;
        if (49 <= input && input <= 58)
            return 8;
        if (58 <= input && input <= 69)
            return 6;
        if (69 <= input && input <= 70)
            return 5;
        if (70 <= input && input <= 101)
            return 6;
        if (101 <= input && input <= 102)
            return 5;
        if (102 <= input && input <= 4294967295)
            return 6;
        return -1;
    case 1:
        if (0 <= input && input <= 43)
            return 6;
        if (43 <= input && input <= 44)
            return 6;
        if (44 <= input && input <= 45)
            return 6;
        if (45 <= input && input <= 46)
            return 6;
        if (46 <= input && input <= 47)
            return 6;
        if (47 <= input && input <= 48)
            return 6;
        if (48 <= input && input <= 49)
            return 6;
        if (49 <= input && input <= 58)
            return 6;
        if (58 <= input && input <= 69)
            return 6;
        if (69 <= input && input <= 70)
            return 6;
        if (70 <= input && input <= 101)
            return 6;
        if (101 <= input && input <= 102)
            return 6;
        if (102 <= input && input <= 4294967295)
            return 6;
        return -1;
    case 2:
        if (0 <= input && input <= 43)
            return 6;
        if (43 <= input && input <= 44)
            return 6;
        if (44 <= input && input <= 45)
            return 6;
        if (45 <= input && input <= 46)
            return 6;
        if (46 <= input && input <= 47)
            return 6;
        if (47 <= input && input <= 48)
            return 6;
        if (48 <= input && input <= 49)
            return 1;
        if (49 <= input && input <= 58)
            return 4;
        if (58 <= input && input <= 69)
            return 6;
        if (69 <= input && input <= 70)
            return 6;
        if (70 <= input && input <= 101)
            return 6;
        if (101 <= input && input <= 102)
            return 6;
        if (102 <= input && input <= 4294967295)
            return 6;
        return -1;
    case 4:
        if (0 <= input && input <= 43)
            return 6;
        if (43 <= input && input <= 44)
            return 6;
        if (44 <= input && input <= 45)
            return 6;
        if (45 <= input && input <= 46)
            return 6;
        if (46 <= input && input <= 47)
            return 6;
        if (47 <= input && input <= 48)
            return 6;
        if (48 <= input && input <= 49)
            return 4;
        if (49 <= input && input <= 58)
            return 4;
        if (58 <= input && input <= 69)
            return 6;
        if (69 <= input && input <= 70)
            return 6;
        if (70 <= input && input <= 101)
            return 6;
        if (101 <= input && input <= 102)
            return 6;
        if (102 <= input && input <= 4294967295)
            return 6;
        return -1;
    case 7:
        if (0 <= input && input <= 43)
            return 6;
        if (43 <= input && input <= 44)
            return 6;
        if (44 <= input && input <= 45)
            return 6;
        if (45 <= input && input <= 46)
            return 6;
        if (46 <= input && input <= 47)
            return 6;
        if (47 <= input && input <= 48)
            return 6;
        if (48 <= input && input <= 49)
            return 9;
        if (49 <= input && input <= 58)
            return 9;
        if (58 <= input && input <= 69)
            return 6;
        if (69 <= input && input <= 70)
            return 6;
        if (70 <= input && input <= 101)
            return 6;
        if (101 <= input && input <= 102)
            return 6;
        if (102 <= input && input <= 4294967295)
            return 6;
        return -1;
    case 5:
        if (0 <= input && input <= 43)
            return 6;
        if (43 <= input && input <= 44)
            return 2;
        if (44 <= input && input <= 45)
            return 6;
        if (45 <= input && input <= 46)
            return 2;
        if (46 <= input && input <= 47)
            return 6;
        if (47 <= input && input <= 48)
            return 6;
        if (48 <= input && input <= 49)
            return 1;
        if (49 <= input && input <= 58)
            return 4;
        if (58 <= input && input <= 69)
            return 6;
        if (69 <= input && input <= 70)
            return 6;
        if (70 <= input && input <= 101)
            return 6;
        if (101 <= input && input <= 102)
            return 6;
        if (102 <= input && input <= 4294967295)
            return 6;
        return -1;
    case 9:
        if (0 <= input && input <= 43)
            return 6;
        if (43 <= input && input <= 44)
            return 6;
        if (44 <= input && input <= 45)
            return 6;
        if (45 <= input && input <= 46)
            return 6;
        if (46 <= input && input <= 47)
            return 6;
        if (47 <= input && input <= 48)
            return 6;
        if (48 <= input && input <= 49)
            return 9;
        if (49 <= input && input <= 58)
            return 9;
        if (58 <= input && input <= 69)
            return 6;
        if (69 <= input && input <= 70)
            return 5;
        if (70 <= input && input <= 101)
            return 6;
        if (101 <= input && input <= 102)
            return 5;
        if (102 <= input && input <= 4294967295)
            return 6;
        return -1;
    default:
        return -1;
    }
}

int transition_character(int state, int input)
{
    fail_if_empty(input);
    switch (state) {
    case 0:
        if (0 <= input && input <= 32)
            return 5;
        if (32 <= input && input <= 39)
            return 5;
        if (39 <= input && input <= 40)
            return 3;
        if (40 <= input && input <= 92)
            return 5;
        if (92 <= input && input <= 93)
            return 5;
        if (93 <= input && input <= 110)
            return 5;
        if (110 <= input && input <= 111)
            return 5;
        if (111 <= input && input <= 116)
            return 5;
        if (116 <= input && input <= 117)
            return 5;
        if (117 <= input && input <= 127)
            return 5;
        if (127 <= input && input <= 4294967295)
            return 5;
        return -1;
    case 5:
        if (0 <= input && input <= 32)
            return 5;
        if (32 <= input && input <= 39)
            return 5;
        if (39 <= input && input <= 40)
            return 5;
        if (40 <= input && input <= 92)
            return 5;
        if (92 <= input && input <= 93)
            return 5;
        if (93 <= input && input <= 110)
            return 5;
        if (110 <= input && input <= 111)
            return 5;
        if (111 <= input && input <= 116)
            return 5;
        if (116 <= input && input <= 117)
            return 5;
        if (117 <= input && input <= 127)
            return 5;
        if (127 <= input && input <= 4294967295)
            return 5;
        return -1;
    case 3:
        if (0 <= input && input <= 32)
            return 5;
        if (32 <= input && input <= 39)
            return 2;
        if (39 <= input && input <= 40)
            return 5;
        if (40 <= input && input <= 92)
            return 2;
        if (92 <= input && input <= 93)
            return 4;
        if (93 <= input && input <= 110)
            return 2;
        if (110 <= input && input <= 111)
            return 2;
        if (111 <= input && input <= 116)
            return 2;
        if (116 <= input && input <= 117)
            return 2;
        if (117 <= input && input <= 127)
            return 2;
        if (127 <= input && input <= 4294967295)
            return 5;
        return -1;
    case 1:
        if (0 <= input && input <= 32)
            return 5;
        if (32 <= input && input <= 39)
            return 5;
        if (39 <= input && input <= 40)
            return 5;
        if (40 <= input && input <= 92)
            return 5;
        if (92 <= input && input <= 93)
            return 5;
        if (93 <= input && input <= 110)
            return 5;
        if (110 <= input && input <= 111)
            return 5;
        if (111 <= input && input <= 116)
            return 5;
        if (116 <= input && input <= 117)
            return 5;
        if (117 <= input && input <= 127)
            return 5;
        if (127 <= input && input <= 4294967295)
            return 5;
        return -1;
    case 2:
        if (0 <= input && input <= 32)
            return 5;
        if (32 <= input && input <= 39)
            return 5;
        if (39 <= input && input <= 40)
            return 1;
        if (40 <= input && input <= 92)
            return 5;
        if (92 <= input && input <= 93)
            return 5;
        if (93 <= input && input <= 110)
            return 5;
        if (110 <= input && input <= 111)
            return 5;
        if (111 <= input && input <= 116)
            return 5;
        if (116 <= input && input <= 117)
            return 5;
        if (117 <= input && input <= 127)
            return 5;
        if (127 <= input && input <= 4294967295)
            return 5;
        return -1;
    case 4:
        if (0 <= input && input <= 32)
            return 5;
        if (32 <= input && input <= 39)
            return 5;
        if (39 <= input && input <= 40)
            return 2;
        if (40 <= input && input <= 92)
            return 5;
        if (92 <= input && input <= 93)
            return 2;
        if (93 <= input && input <= 110)
            return 5;
        if (110 <= input && input <= 111)
            return 2;
        if (111 <= input && input <= 116)
            return 5;
        if (116 <= input && input <= 117)
            return 2;
        if (117 <= input && input <= 127)
            return 5;
        if (127 <= input && input <= 4294967295)
            return 5;
        return -1;
    default:
        return -1;
    }
}

int transition_string(int state, int input)
{
    fail_if_empty(input);
    switch (state) {
    case 0:
        if (0 <= input && input <= 32)
            return 4;
        if (32 <= input && input <= 34)
            return 4;
        if (34 <= input && input <= 35)
            return 2;
        if (35 <= input && input <= 92)
            return 4;
        if (92 <= input && input <= 93)
            return 4;
        if (93 <= input && input <= 127)
            return 4;
        if (127 <= input && input <= 4294967295)
            return 4;
        return -1;
    case 4:
        if (0 <= input && input <= 32)
            return 4;
        if (32 <= input && input <= 34)
            return 4;
        if (34 <= input && input <= 35)
            return 4;
        if (35 <= input && input <= 92)
            return 4;
        if (92 <= input && input <= 93)
            return 4;
        if (93 <= input && input <= 127)
            return 4;
        if (127 <= input && input <= 4294967295)
            return 4;
        return -1;
    case 2:
        if (0 <= input && input <= 32)
            return 4;
        if (32 <= input && input <= 34)
            return 2;
        if (34 <= input && input <= 35)
            return 1;
        if (35 <= input && input <= 92)
            return 2;
        if (92 <= input && input <= 93)
            return 3;
        if (93 <= input && input <= 127)
            return 2;
        if (127 <= input && input <= 4294967295)
            return 4;
        return -1;
    case 1:
        if (0 <= input && input <= 32)
            return 4;
        if (32 <= input && input <= 34)
            return 4;
        if (34 <= input && input <= 35)
            return 4;
        if (35 <= input && input <= 92)
            return 4;
        if (92 <= input && input <= 93)
            return 4;
        if (93 <= input && input <= 127)
            return 4;
        if (127 <= input && input <= 4294967295)
            return 4;
        return -1;
    case 3:
        if (0 <= input && input <= 32)
            return 4;
        if (32 <= input && input <= 34)
            return 2;
        if (34 <= input && input <= 35)
            return 2;
        if (35 <= input && input <= 92)
            return 2;
        if (92 <= input && input <= 93)
            return 2;
        if (93 <= input && input <= 127)
            return 2;
        if (127 <= input && input <= 4294967295)
            return 4;
        return -1;
    default:
        return -1;
    }
}

int transition_dot(int state, int input)
{
    fail_if_empty(input);
    switch (state) {
    case 0:
        if (0 <= input && input <= 46)
            return 2;
        if (46 <= input && input <= 47)
            return 1;
        if (47 <= input && input <= 4294967295)
            return 2;
        return -1;
    case 2:
        if (0 <= input && input <= 46)
            return 2;
        if (46 <= input && input <= 47)
            return 2;
        if (47 <= input && input <= 4294967295)
            return 2;
        return -1;
    case 1:
        if (0 <= input && input <= 46)
            return 2;
        if (46 <= input && input <= 47)
            return 2;
        if (47 <= input && input <= 4294967295)
            return 2;
        return -1;
    default:
        return -1;
    }
}

int transition_lparen(int state, int input)
{
    fail_if_empty(input);
    switch (state) {
    case 0:
        if (0 <= input && input <= 40)
            return 2;
        if (40 <= input && input <= 41)
            return 1;
        if (41 <= input && input <= 4294967295)
            return 2;
        return -1;
    case 2:
        if (0 <= input && input <= 40)
            return 2;
        if (40 <= input && input <= 41)
            return 2;
        if (41 <= input && input <= 4294967295)
            return 2;
        return -1;
    case 1:
        if (0 <= input && input <= 40)
            return 2;
        if (40 <= input && input <= 41)
            return 2;
        if (41 <= input && input <= 4294967295)
            return 2;
        return -1;
    default:
        return -1;
    }
}

int transition_rparen(int state, int input)
{
    fail_if_empty(input);
    switch (state) {
    case 0:
        if (0 <= input && input <= 41)
            return 2;
        if (41 <= input && input <= 42)
            return 1;
        if (42 <= input && input <= 4294967295)
            return 2;
        return -1;
    case 2:
        if (0 <= input && input <= 41)
            return 2;
        if (41 <= input && input <= 42)
            return 2;
        if (42 <= input && input <= 4294967295)
            return 2;
        return -1;
    case 1:
        if (0 <= input && input <= 41)
            return 2;
        if (41 <= input && input <= 42)
            return 2;
        if (42 <= input && input <= 4294967295)
            return 2;
        return -1;
    default:
        return -1;
    }
}

int transition_comma(int state, int input)
{
    fail_if_empty(input);
    switch (state) {
    case 0:
        if (0 <= input && input <= 44)
            return 2;
        if (44 <= input && input <= 45)
            return 1;
        if (45 <= input && input <= 4294967295)
            return 2;
        return -1;
    case 2:
        if (0 <= input && input <= 44)
            return 2;
        if (44 <= input && input <= 45)
            return 2;
        if (45 <= input && input <= 4294967295)
            return 2;
        return -1;
    case 1:
        if (0 <= input && input <= 44)
            return 2;
        if (44 <= input && input <= 45)
            return 2;
        if (45 <= input && input <= 4294967295)
            return 2;
        return -1;
    default:
        return -1;
    }
}

int transition_star(int state, int input)
{
    fail_if_empty(input);
    switch (state) {
    case 0:
        if (0 <= input && input <= 42)
            return 2;
        if (42 <= input && input <= 43)
            return 1;
        if (43 <= input && input <= 4294967295)
            return 2;
        return -1;
    case 2:
        if (0 <= input && input <= 42)
            return 2;
        if (42 <= input && input <= 43)
            return 2;
        if (43 <= input && input <= 4294967295)
            return 2;
        return -1;
    case 1:
        if (0 <= input && input <= 42)
            return 2;
        if (42 <= input && input <= 43)
            return 2;
        if (43 <= input && input <= 4294967295)
            return 2;
        return -1;
    default:
        return -1;
    }
}

int transition_equal(int state, int input)
{
    fail_if_empty(input);
    switch (state) {
    case 0:
        if (0 <= input && input <= 61)
            return 2;
        if (61 <= input && input <= 62)
            return 1;
        if (62 <= input && input <= 4294967295)
            return 2;
        return -1;
    case 2:
        if (0 <= input && input <= 61)
            return 2;
        if (61 <= input && input <= 62)
            return 2;
        if (62 <= input && input <= 4294967295)
            return 2;
        return -1;
    case 1:
        if (0 <= input && input <= 61)
            return 2;
        if (61 <= input && input <= 62)
            return 2;
        if (62 <= input && input <= 4294967295)
            return 2;
        return -1;
    default:
        return -1;
    }
}

int transition_lbrace(int state, int input)
{
    fail_if_empty(input);
    switch (state) {
    case 0:
        if (0 <= input && input <= 123)
            return 2;
        if (123 <= input && input <= 124)
            return 1;
        if (124 <= input && input <= 4294967295)
            return 2;
        return -1;
    case 2:
        if (0 <= input && input <= 123)
            return 2;
        if (123 <= input && input <= 124)
            return 2;
        if (124 <= input && input <= 4294967295)
            return 2;
        return -1;
    case 1:
        if (0 <= input && input <= 123)
            return 2;
        if (123 <= input && input <= 124)
            return 2;
        if (124 <= input && input <= 4294967295)
            return 2;
        return -1;
    default:
        return -1;
    }
}

int transition_rbrace(int state, int input)
{
    fail_if_empty(input);
    switch (state) {
    case 0:
        if (0 <= input && input <= 125)
            return 2;
        if (125 <= input && input <= 126)
            return 1;
        if (126 <= input && input <= 4294967295)
            return 2;
        return -1;
    case 2:
        if (0 <= input && input <= 125)
            return 2;
        if (125 <= input && input <= 126)
            return 2;
        if (126 <= input && input <= 4294967295)
            return 2;
        return -1;
    case 1:
        if (0 <= input && input <= 125)
            return 2;
        if (125 <= input && input <= 126)
            return 2;
        if (126 <= input && input <= 4294967295)
            return 2;
        return -1;
    default:
        return -1;
    }
}

int transition_colon(int state, int input)
{
    fail_if_empty(input);
    switch (state) {
    case 0:
        if (0 <= input && input <= 58)
            return 2;
        if (58 <= input && input <= 59)
            return 1;
        if (59 <= input && input <= 4294967295)
            return 2;
        return -1;
    case 2:
        if (0 <= input && input <= 58)
            return 2;
        if (58 <= input && input <= 59)
            return 2;
        if (59 <= input && input <= 4294967295)
            return 2;
        return -1;
    case 1:
        if (0 <= input && input <= 58)
            return 2;
        if (58 <= input && input <= 59)
            return 2;
        if (59 <= input && input <= 4294967295)
            return 2;
        return -1;
    default:
        return -1;
    }
}

int transition_lbracket(int state, int input)
{
    fail_if_empty(input);
    switch (state) {
    case 0:
        if (0 <= input && input <= 91)
            return 2;
        if (91 <= input && input <= 92)
            return 1;
        if (92 <= input && input <= 4294967295)
            return 2;
        return -1;
    case 2:
        if (0 <= input && input <= 91)
            return 2;
        if (91 <= input && input <= 92)
            return 2;
        if (92 <= input && input <= 4294967295)
            return 2;
        return -1;
    case 1:
        if (0 <= input && input <= 91)
            return 2;
        if (91 <= input && input <= 92)
            return 2;
        if (92 <= input && input <= 4294967295)
            return 2;
        return -1;
    default:
        return -1;
    }
}

int transition_rbracket(int state, int input)
{
    fail_if_empty(input);
    switch (state) {
    case 0:
        if (0 <= input && input <= 93)
            return 2;
        if (93 <= input && input <= 94)
            return 1;
        if (94 <= input && input <= 4294967295)
            return 2;
        return -1;
    case 2:
        if (0 <= input && input <= 93)
            return 2;
        if (93 <= input && input <= 94)
            return 2;
        if (94 <= input && input <= 4294967295)
            return 2;
        return -1;
    case 1:
        if (0 <= input && input <= 93)
            return 2;
        if (93 <= input && input <= 94)
            return 2;
        if (94 <= input && input <= 4294967295)
            return 2;
        return -1;
    default:
        return -1;
    }
}

int transition_plus(int state, int input)
{
    fail_if_empty(input);
    switch (state) {
    case 0:
        if (0 <= input && input <= 43)
            return 2;
        if (43 <= input && input <= 44)
            return 1;
        if (44 <= input && input <= 4294967295)
            return 2;
        return -1;
    case 2:
        if (0 <= input && input <= 43)
            return 2;
        if (43 <= input && input <= 44)
            return 2;
        if (44 <= input && input <= 4294967295)
            return 2;
        return -1;
    case 1:
        if (0 <= input && input <= 43)
            return 2;
        if (43 <= input && input <= 44)
            return 2;
        if (44 <= input && input <= 4294967295)
            return 2;
        return -1;
    default:
        return -1;
    }
}

int transition_minus(int state, int input)
{
    fail_if_empty(input);
    switch (state) {
    case 0:
        if (0 <= input && input <= 45)
            return 2;
        if (45 <= input && input <= 46)
            return 1;
        if (46 <= input && input <= 4294967295)
            return 2;
        return -1;
    case 2:
        if (0 <= input && input <= 45)
            return 2;
        if (45 <= input && input <= 46)
            return 2;
        if (46 <= input && input <= 4294967295)
            return 2;
        return -1;
    case 1:
        if (0 <= input && input <= 45)
            return 2;
        if (45 <= input && input <= 46)
            return 2;
        if (46 <= input && input <= 4294967295)
            return 2;
        return -1;
    default:
        return -1;
    }
}

int transition_slash(int state, int input)
{
    fail_if_empty(input);
    switch (state) {
    case 0:
        if (0 <= input && input <= 47)
            return 2;
        if (47 <= input && input <= 48)
            return 1;
        if (48 <= input && input <= 4294967295)
            return 2;
        return -1;
    case 2:
        if (0 <= input && input <= 47)
            return 2;
        if (47 <= input && input <= 48)
            return 2;
        if (48 <= input && input <= 4294967295)
            return 2;
        return -1;
    case 1:
        if (0 <= input && input <= 47)
            return 2;
        if (47 <= input && input <= 48)
            return 2;
        if (48 <= input && input <= 4294967295)
            return 2;
        return -1;
    default:
        return -1;
    }
}

int transition_lessthan(int state, int input)
{
    fail_if_empty(input);
    switch (state) {
    case 0:
        if (0 <= input && input <= 60)
            return 2;
        if (60 <= input && input <= 61)
            return 1;
        if (61 <= input && input <= 4294967295)
            return 2;
        return -1;
    case 2:
        if (0 <= input && input <= 60)
            return 2;
        if (60 <= input && input <= 61)
            return 2;
        if (61 <= input && input <= 4294967295)
            return 2;
        return -1;
    case 1:
        if (0 <= input && input <= 60)
            return 2;
        if (60 <= input && input <= 61)
            return 2;
        if (61 <= input && input <= 4294967295)
            return 2;
        return -1;
    default:
        return -1;
    }
}

int transition_greaterthan(int state, int input)
{
    fail_if_empty(input);
    switch (state) {
    case 0:
        if (0 <= input && input <= 62)
            return 2;
        if (62 <= input && input <= 63)
            return 1;
        if (63 <= input && input <= 4294967295)
            return 2;
        return -1;
    case 2:
        if (0 <= input && input <= 62)
            return 2;
        if (62 <= input && input <= 63)
            return 2;
        if (63 <= input && input <= 4294967295)
            return 2;
        return -1;
    case 1:
        if (0 <= input && input <= 62)
            return 2;
        if (62 <= input && input <= 63)
            return 2;
        if (63 <= input && input <= 4294967295)
            return 2;
        return -1;
    default:
        return -1;
    }
}

