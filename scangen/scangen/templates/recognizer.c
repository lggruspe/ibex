struct transition_output transition_{{ recognizer.token }}(int q, uint32_t a)
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
        {%- if start+1 == end %}
        if (a == {{ start }})
        {%- else %}
        if ({{ start }} <= a && a < {{ end }})
        {%- endif %}
            return (struct transition_output){ .status = {{ 1 if next_state in recognizer.accepts else 0 }}, .next_state = {{ next_state }} };
        {%- endif %}
        {%- endfor %}
        return (struct transition_output){ .status = -1, .next_state = {{ recognizer.error }} };
    {%- endif %}
    {%- endfor %}
    default:
        return (struct transition_output){ .status = -1, .next_state = {{ recognizer.error }} };
    }
}

struct recognizer {{ recognizer.token|title }}()
{
    return (struct recognizer){
        .token = TOKEN_{{ recognizer.token|upper }},
        .accept = {{ "true" if 0 in recognizer.accepts else "false" }},
        .error = {{ recognizer.error }},
        .transition = transition_{{ recognizer.token|lower }},
    };
}
