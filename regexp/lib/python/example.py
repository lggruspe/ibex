from scangen2 import generate, from_class

class Scanner:
    pipe = r"\|"
    star = r"\*"
    lparen = r"\("
    rparen = r"\)"
    dot = r"\."
    question = r"\?"
    plus = r"\+"
    #symbol = r"\.|\x[0-9]+|."   # TODO not the same as definition in tokens.py
    #interval = r"\[" + symbol + "-" + symbol + r"\]"

generate(from_class(Scanner))
