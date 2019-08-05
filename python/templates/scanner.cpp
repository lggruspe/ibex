struct {{ scanner.token|title }}Scanner: public Scanner {
    using Scanner::Scanner;
    std::string operator()(std::istream& in);
    {
        int c;
        int eof = std::char_traits<char>::eof();
        std::vector<char> checkpoint;
        std::string lexeme;
        goto {{ scanner.start }};
## for state in scanner.transitions
    s{{ state }}:
        c = in.get();
        if (c == eof) {
            goto se;
        }
        lexeme += c;
## if state in scanner.accepts
        checkpoint.clear();
## endif
        checkpoint.push_back(c);
## for transition in scanner.transitions[state]
## set start = transition[0].start
## set end = transition[0].end
## set next_state = transition[1]
        if ({{ start }} <= c && c <= {{ end }}) {
            goto s{{ next_state }};
        }
## endfor
        goto se;
## endfor
    }
};
