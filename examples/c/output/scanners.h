#pragma once
#include <stdio.h>

#define fail_if_empty(input) do {\
    int c = (input);\
    if (c == 0 || c == EOF) return -1;\
} while (0)

enum token {
    token_EMPTY,
    token_IDENTIFIER,
    token_WHITESPACE,
    token_INTEGER,
    token_NUMBER,
    token_CHARACTER,
    token_STRING,
};

struct scanner {
    enum token token;
    int state;
    int start;
    int (*transition)(int, int);
};

struct scanner scanner_create(enum token token, int (*transition)(int, int))
{
    return (struct scanner) {
        .token = token,
        .state = -1,
        .start = 0,
        .transition = transition
    };
}

int transition_empty(int state, int input)
{
    fail_if_empty(input);
    switch (state) {
    case 0:
        return -1;
    default:
        return -1;
    }
}

int transition_identifier(int state, int input)
{
    fail_if_empty(input);
    switch (state) {
    case 0:
        if (65 <= input && input <= 90)
            return 1;
        if (input == 95)
            return 1;
        if (97 <= input && input <= 122)
            return 1;
        return -1;
    case 1:
        if (48 <= input && input <= 57)
            return 1;
        if (65 <= input && input <= 90)
            return 1;
        if (input == 95)
            return 1;
        if (97 <= input && input <= 122)
            return 1;
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
        if (input == 9)
            return 1;
        if (input == 10)
            return 1;
        if (input == 32)
            return 1;
        return -1;
    case 1:
        return -1;
    default:
        return -1;
    }
}

int transition_integer(int state, int input)
{
    fail_if_empty(input);
    switch (state) {
    case 0:
        if (input == 48)
            return 1;
        if (49 <= input && input <= 57)
            return 2;
        return -1;
    case 1:
        return -1;
    case 2:
        if (input == 48)
            return 2;
        if (49 <= input && input <= 57)
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
        if (input == 48)
            return 5;
        if (49 <= input && input <= 57)
            return 1;
        return -1;
    case 5:
        if (input == 46)
            return 7;
        if (input == 69)
            return 2;
        if (input == 101)
            return 2;
        return -1;
    case 1:
        if (input == 46)
            return 7;
        if (input == 48)
            return 1;
        if (49 <= input && input <= 57)
            return 1;
        if (input == 69)
            return 2;
        if (input == 101)
            return 2;
        return -1;
    case 7:
        if (input == 48)
            return 3;
        if (49 <= input && input <= 57)
            return 3;
        return -1;
    case 2:
        if (input == 43)
            return 4;
        if (input == 45)
            return 4;
        if (input == 48)
            return 6;
        if (49 <= input && input <= 57)
            return 8;
        return -1;
    case 4:
        if (input == 48)
            return 6;
        if (49 <= input && input <= 57)
            return 8;
        return -1;
    case 6:
        return -1;
    case 8:
        if (input == 48)
            return 8;
        if (49 <= input && input <= 57)
            return 8;
        return -1;
    case 3:
        if (input == 48)
            return 3;
        if (49 <= input && input <= 57)
            return 3;
        if (input == 69)
            return 2;
        if (input == 101)
            return 2;
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
        if (input == 39)
            return 2;
        return -1;
    case 2:
        if (32 <= input && input <= 38)
            return 3;
        if (40 <= input && input <= 91)
            return 3;
        if (input == 92)
            return 4;
        if (93 <= input && input <= 126)
            return 3;
        return -1;
    case 1:
        return -1;
    case 3:
        if (input == 39)
            return 1;
        return -1;
    case 4:
        if (input == 39)
            return 3;
        if (input == 92)
            return 3;
        if (93 <= input && input <= 126)
            return 3;
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
        if (input == 34)
            return 2;
        return -1;
    case 2:
        if (32 <= input && input <= 33)
            return 2;
        if (input == 34)
            return 1;
        if (35 <= input && input <= 91)
            return 2;
        if (input == 92)
            return 3;
        if (93 <= input && input <= 126)
            return 2;
        return -1;
    case 1:
        return -1;
    case 3:
        if (32 <= input && input <= 33)
            return 2;
        if (input == 34)
            return 2;
        if (35 <= input && input <= 91)
            return 2;
        if (input == 92)
            return 2;
        if (93 <= input && input <= 126)
            return 2;
        return -1;
    default:
        return -1;
    }
}


