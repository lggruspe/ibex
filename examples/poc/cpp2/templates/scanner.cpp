struct {{ scanner.token|title }}: public BaseRecognizer {
    {{ scanner.token|title }}() : BaseRecognizer(Token::{{ scanner.token|upper }}, {{ "true" if 0 in scanner.accepts else "false" }}, {{ scanner.error }}) {}

    std::pair<int, int> next(int q, uint32_t a) const
    {
        switch (q) {
        {%- for state in scanner.transitions %}
        {%- if state != scanner.error %}
        case {{ state }}:
            {%- for transition in scanner.transitions[state] %}
            {%- set start = transition[0].start %}
            {%- set end = transition[0].end %}
            {%- set next_state = transition[1] %}
            {%- if start == 0 %}
            if (a < {{ end }}) {
            {%- elif start+1 == end %}
            if (a == {{ start }}) {
            {%- else %}
            if ({{ start }} <= a && a < {{ end }}) {
            {%- endif %}
                {%- if next_state in scanner.accepts %}
                return {1, {{ next_state }}};
                {%- elif next_state == scanner.error %}
                return {-1, {{ next_state }}};
                {%- else %}
                return {0, {{ next_state }}};
                {%- endif %}
            }
            {%- endfor %}
            return {-1, {{ scanner.error }}};
        {%- endif %}
        {%- endfor %}
        default:
            return {-1, {{ scanner.error }}};
        }
    }
};
