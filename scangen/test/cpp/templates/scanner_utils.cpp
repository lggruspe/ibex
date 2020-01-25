#define pardir_output_scanner_utils_hpp() \
bool ok; \
std::string lexeme; \
{%- for recognizer in scanner %}
{%- if loop.index == 1 %}
if (scanner_name == "{{ recognizer.token }}") \
{%- else %}
else if (scanner_name == "{{ recognizer.token }}") \
{%- endif %}
    std::tie(ok, lexeme) = {{ recognizer.token|title }}().match(); \
{%- endfor %}
else \
    return; \
std::cout << lexeme << std::endl
