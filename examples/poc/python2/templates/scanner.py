class {{ scanner.token|title }}(BaseRecognizer):
    def __init__(self, io=None):
        if not io:
            io = InputStack()
            io.readfile(sys.stdin)
        super().__init__(Token.{{ scanner.token|upper }}, {{ 0 in scanner.accepts }}, {{ scanner.error }}, io)

    def next(self, q, a):
        {%- for state in scanner.transitions %}
        {%- if state != scanner.error %}
        if q == {{ state }}:
            {%- for transition in scanner.transitions[state] %}
            {%- set start = transition[0].start %}
            {%- set end = transition[0].end %}
            {%- set next_state = transition[1] %}
            {%- if next_state != scanner.error %}
            {%- if start+1 == end %}
            if a == {{ start }}:
            {%- else %}
            if {{ start }} <= a < {{ end }}:
            {%- endif %}
                return {{ 1 if next_state in scanner.accepts else 0 }}, {{ next_state }}
            {%- endif %}
            {%- endfor %}
            return -1, 2
        {%- endif %}
        {%- endfor %}
        return -1, 2
