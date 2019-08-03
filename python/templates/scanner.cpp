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
        if ({{ transition.start }} <= c && c <= {{ transition.end }}) {
            goto s{{ scanner.transitions[transition] }};
        }
## endfor
        goto se;
## endfor
    }
};
