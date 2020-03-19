SCANNER = [
{%- for recognizer in scanner %}
    "{{ recognizer.token }}",
{%- endfor %}
]
