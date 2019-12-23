class Scanner:
    pipe = "|"
    star = "*"
    lparen = "("
    rparen = ")"
    lbrackets = "["
    rbrackets = "]"
    dot = "."
    question = "?"
    plus = "+"
    dash = "-"
    caret = "^"
    dollar = "$"
    lbrace = "{"
    rbrace = "}"
    symbol = r"\.|\x[0-9]+|."
    interval = r"\[" + symbol + "-" + symbol + r"\]"
