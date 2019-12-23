import os
import sys

sys.path.append(os.path.abspath("../scangen"))

import scangen as sg

@sg.token("pipe")
def pipe():
    return sg.symbols('|')

@sg.token("star")
def star():
    return sg.symbols('*')

@sg.token("lparen")
def lparen():
    return sg.symbols('(')

@sg.token("rparen")
def rparen():
    return sg.symbols(')')

@sg.token("dot")
def dot():
    return sg.symbols('.')

@sg.token("question")
def question():
    return sg.symbols('?')

@sg.token("plus")
def plus():
    return sg.symbols('+')

def _symbol():
    any_ = sg.isymbols(0, 0xfffffffd)
    backslash = sg.symbols('\\')
    escaped = backslash.concatenation(any_)
    digit = sg.symbols('0', '9')
    digits = digit.concatenation(digit.closure())
    hex_ = backslash.concatenation(sg.symbols('x').concatenation(digits))
    return escaped.union(any_.union(hex_))

@sg.token("interval")
def interval():
    left = sg.symbols('[')
    right = sg.symbols(']')
    mid = sg.symbols('-')
    return left.concatenation(_symbol()).concatenation(mid.concatenation(_symbol())).concatenation(right)

@sg.token("symbol")
def symbol():
    return _symbol()

if __name__ == "__main__":
    sg.generate()
