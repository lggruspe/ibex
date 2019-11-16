int transition_{{ scanner.token }}(int state, int input)
{
    fail_if_empty(input);
    switch (state) {
## for state in scanner.transitions
    case {{ state }}:
## for transition in scanner.transitions[state]
## set start = transition[0].start
## set end = transition[0].end
## set next_state = transition[1]
## if start == end
        if (input == {{ start }})
## else
        if ({{ start }} <= input && input <= {{ end }})
## endif
            return {{ next_state }};
## endfor
        return -1;
## endfor
    default:
        return -1;
    }
}
