#pragma once
#include <lggds/vector.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALL_RECOGNIZERS \
    {%- for scanner in scanners %}
    {{ scanner.token|title }}{% if loop.index != loop.length %}, \{% endif %}
    {%- endfor %}

#define CONSTRUCTORS(...) (recognizer_constructor[]){ __VA_ARGS__, NULL }

#define match_longest(Fp, ...) _match_longest((Fp), CONSTRUCTORS(__VA_ARGS__))

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

vector_register(vuint32_t, uint32_t)

struct input_stack {
    FILE *fp;
    vector(vuint32_t) stack;
    bool done;
};

struct input_stack is_create(FILE *fp)
{
    return (struct input_stack){
        .fp = fp,
        .stack = vector_create(vuint32_t),
        .done = false,
    };
}

void is_destroy(struct input_stack *is)
{
    is->stack = vector_destroy(vuint32_t, is->stack);
    is->done = true;
}

uint32_t is_get(struct input_stack *is)
{
    if (!vector_is_empty(vuint32_t, is->stack)) {
        uint32_t a = vector_peek(vuint32_t, is->stack);
        is->stack = vector_pop(vuint32_t, is->stack);
        return a;
    }
    uint32_t eof = EOF;
    if (is->done) {
        return eof;
    }
    uint32_t a = fgetc(is->fp);
    if (a == eof) {
        is->done = true;
    }
    return a;
}

void is_unget(struct input_stack *is, uint32_t a)
{
    uint32_t eof = EOF;
    if (a == eof) {
        exit(EXIT_FAILURE);
    }
    is->stack = vector_push(vuint32_t, is->stack, a);
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
    char *lexeme;
    int length;
};

vector_register(vint, int)

struct match_output match(struct input_stack *is, struct recognizer *rec)
{
    vector(vint) checkpoint = vector_create(vint);
    vector(vuint32_t) lexeme = vector_create(vuint32_t);
    checkpoint = vector_push(vint, checkpoint, 0);
    uint32_t a = -1;
    uint32_t eof = EOF;
    while (vector_peek(vint, checkpoint) != rec->error && (a = is_get(is)) != eof) {
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
        is_unget(is, a);
    }
    char *s = malloc(sizeof(char) * (lexeme.size+1));
    s[lexeme.size] = '\0';
    for (int i = 0; i < lexeme.size; ++i) {
        s[i] = (char)(vector_get(vuint32_t, lexeme, i));
    }
    int length = lexeme.size;
    vector_destroy(vint, checkpoint);
    vector_destroy(vuint32_t, lexeme);
    return (struct match_output){
        .ok = rec->accept,
        .lexeme = s,        // must be freed by caller
        .length = length,
    };
}

struct scan_output {
    enum token token;
    char *lexeme;
    int length;
};

typedef struct recognizer (*recognizer_constructor)();

struct scan_output _match_longest(struct input_stack *is, recognizer_constructor recs[])
{
    // recs is a null terminated array of function pointers
    enum token token = TOKEN_EMPTY;
    char *lexeme = strdup("");
    int length = 0;
    for (int i = 0; recs[i]; ++i) {
        struct recognizer rec = recs[i]();
        struct match_output m = match(is, &rec);
        if (m.ok && m.length > length) {
            token = rec.token;
            free(lexeme);
            lexeme = strdup(m.lexeme);
            length = m.length;
        }
        for (int i = m.length-1; i >= 0; --i) {
            is_unget(is, m.lexeme[i]);
        }
        free(m.lexeme);
    }
    for (int i = 0; i < length; ++i) {
        is_get(is);
    }
    return (struct scan_output){
        .token = token,
        .lexeme = lexeme,   // must be freed by caller
        .length = length,
    };
}
