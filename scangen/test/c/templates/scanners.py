SCANNERS = [
    {%- for scanner in scanners %}
    "{{ scanner.token|lower }}",
    {%- endfor %}
]
