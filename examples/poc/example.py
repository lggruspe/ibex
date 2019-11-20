"""scangen example."""
import functools
import os
import sys
import scangen as sg

@sg.token("identifier")
def identifier():
    letters = sg.symbols('_').union(sg.symbols('a', 'z')).union(sg.symbols('A', 'Z'))
    return letters.concatenation(letters.union(sg.symbols('0', '9')).closure())

@sg.token("whitespace")
def whitespace():
    return sg.symbols(' ').union(sg.symbols('\t')).union(sg.symbols('\n'))

@sg.token("integer")
def integer():
    zero = sg.symbols('0')
    nonzero = sg.symbols('1', '9')
    nonzero = nonzero.concatenation(zero.union(nonzero).closure())
    return zero.union(nonzero)

@sg.token("number")
def number():
    def integer():
        zero = sg.symbols('0')
        nonzero = sg.symbols('1', '9')
        nonzero = nonzero.concatenation(zero.union(nonzero).closure())
        return zero.union(nonzero)

    digit = sg.symbols('0', '9')
    decimal = sg.symbols('.').concatenation(digit).concatenation(digit.closure())
    sign = sg.optional(sg.symbols('-').union(sg.symbols('+')))
    exponent = (sg.symbols('e').union(sg.symbols('E'))).concatenation(sign).concatenation(integer())
    return integer().concatenation(sg.optional(decimal)).concatenation(sg.optional(exponent))

@sg.token("character")
def character():
    escapeds = [sg.symbols("'"), sg.symbols("\\"), sg.symbols("t"), sg.symbols("n")]
    escape = sg.symbols("\\").concatenation(functools.reduce(lambda a, b: a.union(b), escapeds))
    middle_choices = [sg.isymbols(32, 38), sg.isymbols(40, 91), sg.isymbols(93, 126), escape]
    middle = functools.reduce(lambda a, b: a.union(b), middle_choices)
    return sg.symbols("'").concatenation(middle).concatenation(sg.symbols("'"))

@sg.token("string")
def string():
    char_choices = [sg.isymbols(32, 33), sg.isymbols(35, 91), sg.isymbols(93, 126)]
    char = functools.reduce(lambda a, b: a.union(b), char_choices)
    char = char.union(sg.symbols("\\").concatenation(sg.isymbols(32, 126)))
    return sg.symbols('"').concatenation(char.closure()).concatenation(sg.symbols('"'))

@sg.token("dot")
def dot():
    return sg.symbols('.')

@sg.token("lparen")
def lparen():
    return sg.symbols('(')

@sg.token("rparen")
def rparen():
    return sg.symbols(')')

@sg.token("comma")
def comma():
    return sg.symbols(',')

@sg.token("star")
def star():
    return sg.symbols('*')

@sg.token("equal")
def equal():
    return sg.symbols('=')

@sg.token("lbrace")
def lbrace():
    return sg.symbols('{')

@sg.token("rbrace")
def rbrace():
    return sg.symbols('}')

@sg.token("colon")
def colon():
    return sg.symbols(':')

@sg.token("lbracket")
def lbracket():
    return sg.symbols('[')

@sg.token("rbracket")
def rbracket():
    return sg.symbols(']')

@sg.token("plus")
def plus():
    return sg.symbols('+')

@sg.token("minus")
def minus():
    return sg.symbols('-')

@sg.token("slash")
def slash():
    return sg.symbols('/')

@sg.token("lessthan")
def lessthan():
    return sg.symbols('<')

@sg.token("greaterthan")
def greaterthan():
    return sg.symbols('>')

if __name__ == "__main__":
    sg.generate()
