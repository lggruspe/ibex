#define pardir_output_scanner_utils_h() \
struct match_output m; \
struct recognizer rec; \
{%- for recognizer in scanner %}
{%- if loop.index == 1 %}
if (strcmp(name, "{{ recognizer.token|lower }}") == 0) { \
{%- else %}
} else if (strcmp(name, "{{ recognizer.token|lower }}") == 0) { \
{%- endif %}
    rec = {{ recognizer.token|title }}(); \
    m = match(&is, &rec); \
{%- endfor %}
} else \
    return; \
printf("%s\n", m.lexeme); \
free(m.lexeme)
