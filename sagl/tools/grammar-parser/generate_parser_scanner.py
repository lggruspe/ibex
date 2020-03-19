from os.path import abspath, dirname, join
import sys

BASEDIR = abspath(join(dirname(__file__), "../../.."))
sys.path.append(join(BASEDIR, "codegen"))
sys.path.append(join(BASEDIR, "rnd/python"))
sys.path.append(join(BASEDIR, "sagl/python"))

from codegen import render
from rnd import convert, from_class
import sagl

class Scanner:
    arrow = r"\->"
    dot = r"\."
    identifier = "[_a-zA-Z][_a-zA-Z0-9]*"
    whitespace = "[ \t\n]"

grammar = """
Grammar -> Rules.
Rules   -> Rules Rule.
Rules   -> .
Rule    -> Lhs arrow Rhs dot.
Lhs     -> identifier.
Rhs     -> Rhs identifier.
Rhs     -> .
"""

scanner = render("template.cpp", context={
    "scanner": convert(from_class(Scanner)),
    "config": {"cpp_namespace": "scanner"},
})

parser = render("parser.hpp", context={
    **sagl.parse_grammar(grammar),
    "config": {
        "scanner_namespace": "scanner",
        "parser_namespace": "parser",
        "parser_ignores": ["whitespace"],
    }
})

outdir = abspath(join(dirname(__file__), "../../src/sagl/grammar-parser"))

with open(join(outdir, "scanner.hpp"), "w") as f:
    print(scanner, file=f)

#with open(join(outdir, "parser.hpp"), "w") as f:
#    print(parser, file=f)
