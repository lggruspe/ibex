#pragma once
#include <lggds/vector.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define create_recognizer(...) (struct recognizer) { \
    .token = TOKEN_EMPTY, \
    .accept = false, \
    .error = -1, \
    .transition = NULL, \
    __VA_ARGS__ \
}

enum token {
    TOKEN_EMPTY = 0,
    {%- for scanner in scanners %}
    TOKEN_{{ scanner.token|upper }},
    {%- endfor %}
};

int print_token(FILE *fp, enum token token)
{
    switch (token) {
    case TOKEN_EMPTY:
        return fprintf(fp, "empty");
    {%- for scanner in scanners %}
    case TOKEN_{{ scanner.token|upper }}:
        return fprintf(fp, "{{ scanner.token|lower }}");
    {%- endfor %}
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

{%- for scanner in scanners %}

{% include "scanner.c" %}
{%- endfor %}

struct match_output {
    bool ok;
    char *lexeme;   // must be freed by match caller
};

vector_register(vint, int)
vector_register(vuint32_t, uint32_t)

struct match_output match(FILE *fp, struct recognizer *rec)
{
    vector(vint) checkpoint;
    vector(vuint32_t) lexeme;
    checkpoint = vector_push(vint, checkpoint, 0);
    uint32_t a = -1;
    uint32_t eof = EOF;
    while (vector_peek(vint, checkpoint) != rec->error && (a = fgetc(fp)) != eof) {
        struct transition_output t = rec->transition(vector_peek(vint, checkpoint), a);
        if (t.status == 1) {
            rec->accept = true;
            checkpoint = vector_destroy(vint, checkpoint);
        }
        checkpoint = vector_push(vint, checkpoint, t.next_state);
        lexeme = vector_push(vuint32_t, lexeme, a);
    }
    for (int i = checkpoint.size; i > 1; --i) {
        uint32_t a = vector_peek(vuint32_t, lexeme);
        lexeme = vector_pop(vuint32_t, lexeme);
        fputc(a, fp);
    }
    char *s = malloc(sizeof(char) * (lexeme.size+1));
    s[lexeme.size] = '\0';
    for (int i = 0; i < lexeme.size; ++i) {
        s[i] = (char)(vector_get(vuint32_t, lexeme, i));
    }
    vector_destroy(vint, checkpoint);
    vector_destroy(vuint32_t, lexeme);
    return (struct match_output){
        .ok = rec->accept,
        .lexeme = s
    };
}
