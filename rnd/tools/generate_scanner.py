from os.path import abspath, dirname, join
import sys

BASEDIR = abspath(dirname(__file__))
sys.path.append(join(BASEDIR, "../../codegen"))
sys.path.append(join(BASEDIR, "../python"))

from codegen import render
from rnd import convert, from_class

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
    code = render("template.cpp", context={
        "scanner": convert(from_class(Scanner)),
        "config": config,
    })
    relpath = "../src/regexp/scanner.hpp"
    with open(join(BASEDIR, relpath), "w") as f:
        print(code, file=f)

if __name__ == "__main__":
    main()
