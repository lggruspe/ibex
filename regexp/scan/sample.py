class Scanner:
    pipe = "|"
    star = "*"
    lparen = "("
    rparen = ")"
    dot = "."
    question = "?"
    plus = "+"
    symbol = r"\.|\x[0-9]+|."   # TODO not the same as definition in tokens.py
    interval = r"\[" + symbol + "-" + symbol + r"\]"
