import scanner

def tokenize():
    while True:
        tok, lex = scanner.match_longest(*(scanner.SCANNERS.values()))
        if tok == scanner.Token.EMPTY:
            break
        print(tok, lex)

tokenize()
