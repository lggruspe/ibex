int {{ recognizer.token }}(int *q, CodePoint a)
{
    switch (*q) {
    {%- for state in recognizer.transitions %}
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
        {%- endif -%}
            { *q = {{ next_state }}; return {{ -1 if next_state == recognizer.error else 1 if next_state in recognizer.accepts else 0 }}; }
        {%- endif %}
        {%- endfor %}
        return -1;
    {%- endfor %}
    default:
        assert(0);
    }
}
