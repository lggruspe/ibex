from os.path import abspath, dirname, join
import sys

sys.path.append(abspath(join(dirname(__file__), "../../../codegen")))
sys.path.append(abspath(join(dirname(__file__), "..")))
sys.path.append(abspath(join(dirname(__file__), "../../../rnd/python")))

import codegen
from grammarp import parse_grammar
import rnd

m = parse_grammar("""
Grammar -> Rules.
Rules   -> Rules Rule.
Rules   -> .
Rule    -> Lhs arrow Rhs dot.
Lhs     -> identifier.
Rhs     -> Rhs identifier.
Rhs     -> .
""")

class Scanner:
    arrow = r"\->"
    identifier = "[_a-zA-Z][_a-zA-Z0-9]"
    dot = r"\."

scanner = codegen.render("template.cpp", context={
    "scanner": rnd.convert(rnd.from_class(Scanner)),
    "config": {
        "cpp_namespace": "scanner",
    }
})

parser = codegen.render("parser.hpp", context={
    "table": m["table"],
    "grammar": m["grammar"],
    "config": {
        "scanner_namespace": "scanner",
    },
})

with open("scanner.hpp", "w") as f:
    print(scanner, file=f)

with open("parser.hpp", "w") as f:
    print(parser, file=f)
