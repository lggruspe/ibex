struct {{ recognizer.token|title }}: public BaseRecognizer {
    {{ recognizer.token|title }}() : BaseRecognizer("{{ recognizer.token }}", {{ "true" if 0 in recognizer.accepts else "false" }}, {{ recognizer.error }}) {}

    std::pair<int, int> next(int q, uint32_t a) const
    {
        switch (q) {
        {%- for state in recognizer.transitions %}
        {%- if state != recognizer.error %}
        case {{ state }}:
            {%- for transition in recognizer.transitions[state] %}
            {%- set start = transition[0].start %}
            {%- set end = transition[0].end %}
            {%- set next_state = transition[1] %}
            {%- if next_state != recognizer.error %}
            {%- if start == 0 %}
            if (a < {{ end }})
            {%- elif start+1 == end %}
            if (a == {{ start }})
            {%- else %}
            if ({{ start }} <= a && a < {{ end }})
            {%- endif %}
                {%- if next_state in recognizer.accepts %}
                return {1, {{ next_state }}};
                {%- else %}
                return {0, {{ next_state }}};
                {%- endif %}
            {%- endif %}
            {%- endfor %}
            return {-1, {{ recognizer.error }}};
        {%- endif %}
        {%- endfor %}
        default:
            return {-1, {{ recognizer.error }}};
        }
    }
};
