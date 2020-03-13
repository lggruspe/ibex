import os
import sys

pardir = os.path.join(os.path.dirname(__file__), os.path.pardir)
sys.path.append(os.path.abspath(pardir))

from scangen import render, from_class

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

config = {"cpp_namespace": "scanner"}

def render_template(template, output, directory=None):
    code = render(from_class(Scanner), entrypoint=template, config=config, directory=directory)
    with open(output, "w") as f:
        print(code, file=f)

path = os.path.abspath("templates")
render_template("template.c", "c/scanner.h")
render_template("template.cpp", "cpp/scanner.hpp")
render_template("template.py", "python/scanner.py")
render_template("template.dot", "dot/scanner.dot")
render_template("scanner_utils.c", "c/scanner_utils.h", directory=path)
render_template("scanner_utils.cpp", "cpp/scanner_utils.hpp", directory=path)
render_template("scanner.py", "c/scanner.py", directory=path)
render_template("scanner.py", "cpp/scanner.py", directory=path)
