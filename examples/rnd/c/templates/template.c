#pragma once
#include <stdio.h>

#define fail_if_empty(input) do {\
    int c = (input);\
    if (c == 0 || c == EOF) return -1;\
} while (0)

enum token {
## for scanner in scanners
    token_{{ scanner.token|upper }},
## endfor
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

## for scanner in scanners
{% include "scanner.c" %}

## endfor
