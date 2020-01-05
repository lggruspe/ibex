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
    __cescaped = r"\\('|\\|t|n)"
    __character = rf"[\x20-\x26]|[\x28-\x5b]|[\x5d-\x7e]|({__cescaped})"
    character = "'({__character})'"
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

generate(from_class(Scanner))
