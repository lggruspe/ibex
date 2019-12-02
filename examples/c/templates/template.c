#pragma once
#include <stdbool.h>
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
    {%- for scanner in scanners %}
    TOKEN_{{ scanner.token|upper }},
    {%- endfor %}
};

struct recognizer {
    enum token token;
    bool accept;
    int error;
    int (*transition)(int, int);
};

{% for scanner in scanners %}
{% include "scanner.c" %}
{% endfor %}
