import os
import sys

sys.path.append(os.path.abspath("../../../sgv2"))

from scangen2 import generate, from_class

class Scanner:
    pipe = r"\|"
    star = r"\*"
    lparen = r"\("
    rparen = r"\)"
    dot = r"\."
    question = r"\?"
    plus = r"\+"
    lbracket = r"\["
    rbracket = r"\]"
    dash = r"\-"
    __escaped = r"\\(\\|\||\*|\(|\)|\.|\?|\+|\[|\]|\-)"
    __hex = r"\\[xX][0-9a-fA-F]+"
    __not_escaped = r"[\x0-\x5a\x5e-\xfffffffe]"
    symbol = rf"({__escaped})|({__hex})|({__not_escaped})"

generate(from_class(Scanner))
