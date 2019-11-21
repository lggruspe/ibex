class {{ scanner.token|title }}(BaseRecognizer):
    def __init__(self, io=InputStack()):
        super().__init__(Token.{{ scanner.token|upper }}, {{ 0 in scanner.accepts }}, {{ scanner.error }}, io)

    def next(self, q, a):
        {%- for state in scanner.transitions %}
        {%- if state != scanner.error %}
        if q == {{ state }}:
            {%- for transition in scanner.transitions[state] %}
            {%- set start = transition[0].start %}
            {%- set end = transition[0].end %}
            {%- set next_state = transition[1] %}
            if {{ start }} <= a < {{ end }}:
                {%- if next_state == scanner.error %}
                return -1, {{ next_state }}
                    {%- elif next_state in scanner.accepts %}
                return 1, {{ next_state }}
                {%- else %}
                return 0, {{ next_state }}
                {%- endif -%}
            {%- endfor %}
        {%- endif %}
        {%- endfor %}
        return -1, 2
