from os.path import abspath, dirname, join, pardir
import sys

sys.path.append(abspath(join(dirname(__file__), pardir)))
sys.path.append(abspath(join(dirname(__file__), pardir, "codegen")))

import codegen
import rnd

class Scanner:
    identifier = "[_a-zA-Z][0-9a-zA-Z_]*"
    whitespace = " |\t|\n"
    __integer = "0|[1-9][0-9]*"
    number = rf"({__integer})(\.[0-9]+)?([eE][\-\+]?({__integer}))?"
    __cescaped = r"\\('|\\|t|n)"
    __character = rf"[\x20-\x26]|[\x28-\x5b]|[\x5d-\x7e]|({__cescaped})"
    character = f"'({__character})'"
    __sescaped = r"\\[\x20-\x7e]"
    __string = rf"[\x20-\x21]|[\x23-\x5b]|[\x5d-\x7e]|({__sescaped})"
    string = f'"({__string})*"'
    dot = r"\."
    lparen = r"\("
    rparen = r"\)"
    comma = ","
    star = r"\*"
    equal = "="
    lbrace = "{"
    rbrace = "}"
    colon = ":"
    lbracket = r"\["
    rbracket = r"\]"
    plus = r"\+"
    minus = r"\-"
    slash = "/"
    lessthan = "<"
    greaterthan = ">"

def render_template(template, output, directory=None):
    code = codegen.render(template, context={
        "scanner": rnd.convert(rnd.from_class(Scanner)),
        "config": {
            "cpp_namespace": "scanner",
        }
    }, directory=directory)
    with open(output, "w") as f:
        print(code, file=f)

path = abspath(join(dirname(__file__), "templates"))
render_template("scanner.c", "c/scanner.h")
render_template("scanner.cpp", "cpp/scanner.hpp")
render_template("scanner.py", "python/scanner.py")
render_template("template.dot", "dot/scanner.dot")
render_template("scanner_utils.c", "c/scanner_utils.h", directory=path)
render_template("scanner_utils.cpp", "cpp/scanner_utils.hpp", directory=path)
render_template("scanner.py", "c/scanner.py", directory=path)
render_template("scanner.py", "cpp/scanner.py", directory=path)
