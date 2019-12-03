#define pardir_output_scanner_utils_h() \
struct match_output m; \
struct recognizer rec; \
{%- for scanner in scanners %}
{%- if loop.index == 1 %}
if (strcmp(name, "{{ scanner.token|lower }}") == 0) { \
{%- else %}
} else if (strcmp(name, "{{ scanner.token|lower }}") == 0) { \
{%- endif %}
    rec = {{ scanner.token|title }}(); \
    m = match(stdin, &rec); \
{%- endfor %}
} else \
    return; \
printf("%s\n", m.lexeme); \
free(m.lexeme)
