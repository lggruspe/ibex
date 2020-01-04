from scangen2 import generate, from_class

class Scanner:
    pipe = r"\|"
    star = r"\*"
    lparen = r"\("
    rparen = r"\)"
    dot = r"\."
    question = r"\?"
    plus = r"\+"
    __escaped = r"\\(\\|\||\*|\(|\)|\.|\?|\+|\[|\])"
    __hex = r"\\[xX][0-9a-fA-F]+"
    #__not_escaped = r"[\x0-\x5a\x5e-\xfffffffe]"
    #symbol = rf"({__escaped})|({__hex})|({__not_escaped})"
    #interval = r"\[" + symbol + "-" + symbol + r"\]"

generate(from_class(Scanner))
