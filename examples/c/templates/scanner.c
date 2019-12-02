struct transition_output transition_{{ scanner.token }}(int q, uint32_t a)
{
    switch (q) {
    {%- for state in scanner.transitions %}
    {%- if state != scanner.error %}
    case {{ state }}:
        {%- for transition in scanner.transitions[state] %}
        {%- set start = transition[0].start %}
        {%- set end = transition[0].end %}
        {%- set next_state = transition[1] %}
        {%- if next_state != scanner.error %}
        if ({{ start }} <= a && a < {{ end }})
            return { .status = {{ 1 if next_state in scanner.accepts else 0 }}, .next_state = {{ next_state }} }; 
        {%- endif %}
        {%- endfor %}
        return { .status = -1, .next_state = {{ scanner.error }} };
    {%- endif %}
    {%- endfor %}
    default:
        return { .status = -1, .next_state = {{ scanner.error }} };
    }
}
