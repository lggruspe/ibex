import functools
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
    backslash = sg.symbols('\\')
    pipe = sg.symbols('|')
    star = sg.symbols('*')
    lparen = sg.symbols('(')
    rparen = sg.symbols(')')
    dot = sg.symbols('.')
    question = sg.symbols('?')
    plus = sg.symbols('+')
    lbracket = sg.symbols('[')
    rbracket = sg.symbols(']')
    escaped = functools.reduce(lambda a, b: a.union(b),
        [backslash, pipe, star, lparen, rparen, dot, question, plus, lbracket,
            rbracket])
    escaped = backslash.concatenation(escaped)

    digit = sg.symbols('0', '9')
    af = sg.symbols('a', 'f')
    AF = sg.symbols('A', 'F')
    hexdigit = digit.union(af).union(AF)
    hexdigits = hexdigit.concatenation(hexdigit.closure())
    x = sg.symbols('x').union(sg.symbols('X'))
    hex_ = backslash.concatenation(x.concatenation(hexdigits))

    # not_escaped excludes [, \\ and ]
    not_escaped = sg.isymbols(0, 90).union(sg.isymbols(94, 0xfffffffe))
    return escaped.union(hex_).union(not_escaped)

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
