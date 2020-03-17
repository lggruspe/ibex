from os.path import abspath, dirname, join
import sys

sys.path.append(abspath(join(dirname(__file__), "../../codegen")))

import codegen
from grammarp import parse_grammar

m = parse_grammar("""
Grammar -> Rules.
Rules   -> Rules Rule.
Rules   -> .
Rule    -> Lhs arrow Rhs dot.
Lhs     -> identifier.
Rhs     -> Rhs identifier.
Rhs     -> .
""")

doc = codegen.render("parser.hpp", context={
    "table": m["table"],
    "grammar": m["grammar"],
})

print(doc)
