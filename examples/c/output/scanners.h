#pragma once
#include <lggds/vector.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

struct recognizer Identifier()
{
    return (struct recognizer){
        .token = TOKEN_IDENTIFIER,
        .accept = false,
        .error = 2,
        .transition = transition_identifier,
    };
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

struct recognizer Whitespace()
{
    return (struct recognizer){
        .token = TOKEN_WHITESPACE,
        .accept = false,
        .error = 2,
        .transition = transition_whitespace,
    };
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

struct recognizer Number()
{
    return (struct recognizer){
        .token = TOKEN_NUMBER,
        .accept = false,
        .error = 6,
        .transition = transition_number,
    };
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

struct recognizer Character()
{
    return (struct recognizer){
        .token = TOKEN_CHARACTER,
        .accept = false,
        .error = 5,
        .transition = transition_character,
    };
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

struct recognizer String()
{
    return (struct recognizer){
        .token = TOKEN_STRING,
        .accept = false,
        .error = 4,
        .transition = transition_string,
    };
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

struct recognizer Dot()
{
    return (struct recognizer){
        .token = TOKEN_DOT,
        .accept = false,
        .error = 2,
        .transition = transition_dot,
    };
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

struct recognizer Lparen()
{
    return (struct recognizer){
        .token = TOKEN_LPAREN,
        .accept = false,
        .error = 2,
        .transition = transition_lparen,
    };
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

struct recognizer Rparen()
{
    return (struct recognizer){
        .token = TOKEN_RPAREN,
        .accept = false,
        .error = 2,
        .transition = transition_rparen,
    };
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

struct recognizer Comma()
{
    return (struct recognizer){
        .token = TOKEN_COMMA,
        .accept = false,
        .error = 2,
        .transition = transition_comma,
    };
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

struct recognizer Star()
{
    return (struct recognizer){
        .token = TOKEN_STAR,
        .accept = false,
        .error = 2,
        .transition = transition_star,
    };
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

struct recognizer Equal()
{
    return (struct recognizer){
        .token = TOKEN_EQUAL,
        .accept = false,
        .error = 2,
        .transition = transition_equal,
    };
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

struct recognizer Lbrace()
{
    return (struct recognizer){
        .token = TOKEN_LBRACE,
        .accept = false,
        .error = 2,
        .transition = transition_lbrace,
    };
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

struct recognizer Rbrace()
{
    return (struct recognizer){
        .token = TOKEN_RBRACE,
        .accept = false,
        .error = 2,
        .transition = transition_rbrace,
    };
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

struct recognizer Colon()
{
    return (struct recognizer){
        .token = TOKEN_COLON,
        .accept = false,
        .error = 2,
        .transition = transition_colon,
    };
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

struct recognizer Lbracket()
{
    return (struct recognizer){
        .token = TOKEN_LBRACKET,
        .accept = false,
        .error = 2,
        .transition = transition_lbracket,
    };
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

struct recognizer Rbracket()
{
    return (struct recognizer){
        .token = TOKEN_RBRACKET,
        .accept = false,
        .error = 2,
        .transition = transition_rbracket,
    };
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

struct recognizer Plus()
{
    return (struct recognizer){
        .token = TOKEN_PLUS,
        .accept = false,
        .error = 2,
        .transition = transition_plus,
    };
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

struct recognizer Minus()
{
    return (struct recognizer){
        .token = TOKEN_MINUS,
        .accept = false,
        .error = 2,
        .transition = transition_minus,
    };
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

struct recognizer Slash()
{
    return (struct recognizer){
        .token = TOKEN_SLASH,
        .accept = false,
        .error = 2,
        .transition = transition_slash,
    };
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

struct recognizer Lessthan()
{
    return (struct recognizer){
        .token = TOKEN_LESSTHAN,
        .accept = false,
        .error = 2,
        .transition = transition_lessthan,
    };
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

struct recognizer Greaterthan()
{
    return (struct recognizer){
        .token = TOKEN_GREATERTHAN,
        .accept = false,
        .error = 2,
        .transition = transition_greaterthan,
    };
}

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
