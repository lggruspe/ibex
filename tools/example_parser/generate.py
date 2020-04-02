from os.path import abspath, dirname, join, pardir
import sys

BASEDIR = abspath(join(dirname(__file__), pardir, pardir))
sys.path.append(join(BASEDIR, "python"))
sys.path.append(join(BASEDIR, "python/codegen"))

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

scanner_h = codegen.render("cscanner/scanner.h", context={})

scanner_c = codegen.render("cscanner/scanner.c", context={
    "scanner": rnd.convert(rnd.from_class(Scanner)),
    "config": None,
})

parser_h = codegen.render("cparser/parser.h", context={})

parser_cpp = codegen.render("cparser/parser.cpp", context={
    **sagl.parse_grammar(grammar),
})

with open(join(BASEDIR, "examples", "scanner.h"), "w") as f:
    print(scanner_h, file=f)

with open(join(BASEDIR, "examples", "scanner.c"), "w") as f:
    print(scanner_c, file=f)

with open(join(BASEDIR, "examples", "parser.h"), "w") as f:
    print(parser_h, file=f)

with open(join(BASEDIR, "examples", "parser.cpp"), "w") as f:
    print(parser_cpp, file=f)
