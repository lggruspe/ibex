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
    {%- for scanner in scanners %}
    TOKEN_{{ scanner.token|upper }},
    {%- endfor %}
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

{%- for scanner in scanners %}

{% include "scanner.c" %}
{%- endfor %}
