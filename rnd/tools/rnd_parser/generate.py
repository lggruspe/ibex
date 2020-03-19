from os.path import abspath, dirname, join
import sys

BASEDIR = abspath(join(dirname(__file__), "../.."))
sys.path.append(join(BASEDIR, "codegen"))
sys.path.append(join(BASEDIR, "rnd/python"))

from codegen import render
from rnd import convert, from_class
import sagl

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

grammar = """
start -> expr.
expr -> expr pipe term.
expr -> term.
term -> term factor.
term -> factor.
factor -> value star.
factor -> value plus.
factor -> value question.
factor -> value.
value -> simple.
value -> compound.
simple -> dot.
simple -> symbol.
compound -> lparen expr rparen.
compound -> lbracket list rbracket.
list -> list element.
list -> element.
element -> symbol.
element -> symbol dash symbol.
"""

scanner = render("template.cpp", context={
    "scanner": convert(from_class(Scanner)),
    "config": {
        "cpp_namespace": "scanner",
    }
})

parser = render("parser.hpp", context={
    **sagl.parse_grammar(grammar),
    "config": {
        "scanner_namespace": "scanner",
        "parser_namespace": "parser",
    }
})

outdir = join(BASEDIR, "rnd/src/rnd/parser")

with open(join(outdir, "scanner.hpp"), "w") as f:
    print(scanner, file=f)

with open(join(outdir, "parser.hpp"), "w") as f:
    print(parser, file=f)
