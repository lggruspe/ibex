import os
import sys

pardir = os.path.join(os.path.dirname(__file__), os.path.pardir)
sys.path.append(os.path.abspath(pardir))

from scangen2 import generate, from_class

class Scanner:
    identifier = "[_a-zA-Z][0-9a-zA-Z_]*"
    whitespace = " |\t|\n"
    __integer = "0|[1-9][0-9]*"
    number = rf"({__integer})(\.[0-9]+)?([eE][\-\+]?({__integer}))?"
    #character = "'({__character})'"
    #string = '"({__character})*"'
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

generate(from_class(Scanner))
