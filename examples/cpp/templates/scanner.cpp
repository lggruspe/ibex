struct {{ scanner.token|title }}Scanner: public Scanner {
    //using Scanner::Scanner;
    {{ scanner.token|title }}Scanner() : Scanner(Token::{{ scanner.token }})
    {
## if 0 in scanner.accepts
        accept = 0;
## else
        accept = -1;
## endif
    }

    bool next(int c)
    {
        int eof = std::char_traits<char>::eof();
        if (c == eof && state() != -1) {
            checkpoint.push_back(-1);
        }
        switch (state()) {
## for state in scanner.transitions
        case {{state}}:
## if state in scanner.accepts
            accept = state();
            checkpoint.clear();
## endif
## for transition in scanner.transitions[state]
## set start = transition[0].start
## set end = transition[0].end
## set next_state = transition[1]
            if ({{start}} <= c && c <= {{end}}) {
                checkpoint.push_back({{next_state}});
## if state == -1
                return false;
## else
                return true;
## endif
            }
## endfor
            checkpoint.push_back(-1);
            return false;
## endfor
        default:
            return false;
        }
    }
};
