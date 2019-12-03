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

#define match_first(Fp, ...) _match_first((Fp), CONSTRUCTORS(__VA_ARGS__))

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

struct scan_output _match_first(FILE *fp, recognizer_constructor recs[])
{
    for (int i = 0; recs[i]; ++i) {
        struct recognizer rec = recs[i]();
        struct match_output m = match(fp, &rec);
        if (m.ok) {
            return (struct scan_output){
                .token = rec.token,
                .lexeme = m.lexeme ? m.lexeme : strdup(""),
                .length = m.length,
            };
        }
        free(m.lexeme);
    }
    return (struct scan_output){
        .token = TOKEN_EMPTY,
        .lexeme = strdup(""),   // must be freed by caller
        .length = 0,
    };
}

struct scan_output _match_longest(FILE *fp, recognizer_constructor recs[])
{
    // recs is a null terminated array of function pointers
    enum token token = TOKEN_EMPTY;
    char *lexeme = NULL;
    int length = 0;
    for (int i = 0; recs[i]; ++i) {
        struct recognizer rec = recs[i]();
        struct match_output m = match(fp, &rec);
        if (m.ok && m.length > length) {
            token = rec.token;
            lexeme = strdup(m.lexeme);
            length = m.length;
        }
        for (int i = m.length-1; i >= 0; --i) {
            fputc(m.lexeme[i], fp);
        }
        free(m.lexeme);
    }
    for (int i = 0; i < length; ++i) {
        fgetc(fp);
    }
    return (struct scan_output){
        .token = token,
        .lexeme = lexeme,   // must be freed by caller if not null
        .length = length,
    };
}
