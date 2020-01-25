class {{ recognizer.token|title }}(BaseRecognizer):
    def __init__(self):
        super().__init__(Token.{{ recognizer.token|upper }}, {{ 0 in recognizer.accepts }}, {{ recognizer.error }})

    def next(self, q, a):
        {%- for state in recognizer.transitions %}
        {%- if state != recognizer.error %}
        if q == {{ state }}:
            {%- for transition in recognizer.transitions[state] %}
            {%- set start = transition[0].start %}
            {%- set end = transition[0].end %}
            {%- set next_state = transition[1] %}
            {%- if next_state != recognizer.error %}
            {%- if start+1 == end %}
            if a == {{ start }}:
            {%- else %}
            if {{ start }} <= a < {{ end }}:
            {%- endif %}
                return {{ 1 if next_state in recognizer.accepts else 0 }}, {{ next_state }}
            {%- endif %}
            {%- endfor %}
            return -1, {{ recognizer.error }}
        {%- endif %}
        {%- endfor %}
        return -1, {{ recognizer.error }}
