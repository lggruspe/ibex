#define pardir_output_scanner_utils_hpp() \
bool ok; \
std::string lexeme; \
{%- for scanner in scanners %}
{%- if loop.index == 1 %}
if (scanner_name == "{{ scanner.token }}") \
{%- else %}
else if (scanner_name == "{{ scanner.token }}") \
{%- endif %}
    std::tie(ok, lexeme) = {{ scanner.token|title }}().match(); \
{%- endfor %}
else \
    return; \
std::cout << lexeme << std::endl
