SCANNER = [
    {%- for recognizer in scanner %}
    "{{ recognizer.token|lower }}",
    {%- endfor %}
]
