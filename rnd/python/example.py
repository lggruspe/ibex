from os.path import abspath, dirname, join
import sys

sys.path.append(abspath(join(dirname(__file__), "../../codegen")))

import rnd
import codegen

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

doc = codegen.render("template.cpp", context={
    "scanner": rnd.convert(rnd.from_class(Scanner)),
    "config": {
        "cpp_namespace": "scanner"
    }
})
print(doc)
