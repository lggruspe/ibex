from os.path import abspath, dirname, join
import sys

BASEDIR = abspath(join(dirname(__file__), "../.."))
sys.path.append(join(BASEDIR, "codegen"))
sys.path.append(join(BASEDIR, "rnd/python"))

import codegen
import rnd
import sagl

class Scanner:
    a = "a"
    b = "b"
    whitespace = "[ \t\n]"

grammar = """
S -> A.
A -> a A b.
A -> .
"""

scanner = codegen.render("template.cpp", context={
    "scanner": rnd.convert(rnd.from_class(Scanner)),
    "config": None,
})

parser = codegen.render("parser.hpp", context={
    **sagl.parse_grammar(grammar),
})

with open("scanner.hpp", "w") as f:
    print(scanner, file=f)

with open("parser.hpp", "w") as f:
    print(parser, file=f)
