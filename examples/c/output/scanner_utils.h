#define pardir_output_scanner_utils_h() \
struct match_output m; \
struct recognizer rec; \
if (strcmp(name, "identifier") == 0) { \
    rec = Identifier(); \
    m = match(stdin, &rec); \
} else if (strcmp(name, "whitespace") == 0) { \
    rec = Whitespace(); \
    m = match(stdin, &rec); \
} else if (strcmp(name, "number") == 0) { \
    rec = Number(); \
    m = match(stdin, &rec); \
} else if (strcmp(name, "character") == 0) { \
    rec = Character(); \
    m = match(stdin, &rec); \
} else if (strcmp(name, "string") == 0) { \
    rec = String(); \
    m = match(stdin, &rec); \
} else if (strcmp(name, "dot") == 0) { \
    rec = Dot(); \
    m = match(stdin, &rec); \
} else if (strcmp(name, "lparen") == 0) { \
    rec = Lparen(); \
    m = match(stdin, &rec); \
} else if (strcmp(name, "rparen") == 0) { \
    rec = Rparen(); \
    m = match(stdin, &rec); \
} else if (strcmp(name, "comma") == 0) { \
    rec = Comma(); \
    m = match(stdin, &rec); \
} else if (strcmp(name, "star") == 0) { \
    rec = Star(); \
    m = match(stdin, &rec); \
} else if (strcmp(name, "equal") == 0) { \
    rec = Equal(); \
    m = match(stdin, &rec); \
} else if (strcmp(name, "lbrace") == 0) { \
    rec = Lbrace(); \
    m = match(stdin, &rec); \
} else if (strcmp(name, "rbrace") == 0) { \
    rec = Rbrace(); \
    m = match(stdin, &rec); \
} else if (strcmp(name, "colon") == 0) { \
    rec = Colon(); \
    m = match(stdin, &rec); \
} else if (strcmp(name, "lbracket") == 0) { \
    rec = Lbracket(); \
    m = match(stdin, &rec); \
} else if (strcmp(name, "rbracket") == 0) { \
    rec = Rbracket(); \
    m = match(stdin, &rec); \
} else if (strcmp(name, "plus") == 0) { \
    rec = Plus(); \
    m = match(stdin, &rec); \
} else if (strcmp(name, "minus") == 0) { \
    rec = Minus(); \
    m = match(stdin, &rec); \
} else if (strcmp(name, "slash") == 0) { \
    rec = Slash(); \
    m = match(stdin, &rec); \
} else if (strcmp(name, "lessthan") == 0) { \
    rec = Lessthan(); \
    m = match(stdin, &rec); \
} else if (strcmp(name, "greaterthan") == 0) { \
    rec = Greaterthan(); \
    m = match(stdin, &rec); \
} else \
    return; \
printf("%s\n", m.lexeme); \
free(m.lexeme)
