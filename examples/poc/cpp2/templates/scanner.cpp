struct {{ scanner.token|title }}Scanner: public Scanner {
    {{ scanner.token|title }}Scanner() : Scanner(Token::{{ scanner.token }})
    {
        error = {{ scanner.error }};
    }

    bool next(uint32_t c)
    {
        switch (state()) {
## for state in scanner.transitions
        case {{state}}:
## for transition in scanner.transitions[state]
## set start = transition[0].start
## set end = transition[0].end
## set next_state = transition[1]
            if ({{ start }} <= c && c < {{ end }})
## if next_state in scanner.accepts
                return change_state({{ next_state }}, true);
## else
                return change_state({{ next_state }});
## endif
## endfor
            return change_state(error);
## endfor
        default:
            return change_state(error);
        }
    }
};
