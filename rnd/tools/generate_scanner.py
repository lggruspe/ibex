import os.path as op
import sys

sys.path.append(op.abspath(op.join(op.dirname(__file__), "../../scangen")))

from scangen import render, from_class

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

config = {"cpp_namespace": "scanner"}

def main():
    code = render(from_class(Scanner), "template.cpp", config=config)
    relpath = "../src/regexp/scanner.hpp"
    abspath = op.abspath(op.join(op.dirname(__file__), relpath))
    with open(abspath, "w") as f:
        print(code, file=f)

if __name__ == "__main__":
    main()
