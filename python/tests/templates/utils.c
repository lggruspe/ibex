#define pardir_output_scanner_utils_h() \
char *lexeme = NULL; \
{%- for recognizer in scanner %}
{%- if loop.index == 1 %}
if (strcmp(name, "{{ recognizer.token|lower }}") == 0) { \
{%- else %}
} else if (strcmp(name, "{{ recognizer.token|lower }}") == 0) { \
{%- endif %}
    int n = compute({{ recognizer.token|lower }}, s); \
    lexeme = strndup(s, n); \
{%- endfor %}
} \
if (lexeme) { \
    printf("%s\n", lexeme); \
    free(lexeme); \
}
