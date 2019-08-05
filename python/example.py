import scangen as sg
rnd = sg.rnd
import functools

@sg.token("empty")
def empty():
    return sg.symbols('\0')

@sg.token("identifier")
def identifier():
    letters = sg.symbols('_').union(sg.symbols('a', 'z')).union(sg.symbols('A', 'Z'))
    return letters.concatenation(letters.union(sg.symbols('0', '9')).closure())

@sg.token("whitespace")
def whitespace():
    return sg.symbols(' ').union(sg.symbols('\t')).union(sg.symbols('\n'))

@sg.token("integer")
def integer():
    return sg.symbols('0').union(sg.symbols('1', '9').concatenation(sg.symbols('0', '9').closure()))

@sg.token("number")
def number():
    def integer():
        return sg.symbols('0').union(sg.symbols('1', '9').concatenation(sg.symbols('0', '9').closure()))

    digit = sg.symbols('0', '9')
    decimal = sg.symbols('.').concatenation(digit).concatenation(digit.closure())
    sign = sg.optional(sg.symbols('-').union(sg.symbols('+')))
    exponent = (sg.symbols('e').union(sg.symbols('E'))).concatenation(sign).concatenation(integer())
    return integer().concatenation(sg.optional(decimal)).concatenation(sg.optional(exponent))

@sg.token("character")
def character():
    escape = sg.symbols("\\").concatenation(functools.reduce(lambda a, b: a.union(b),
        [sg.symbols("'"), sg.symbols("\\"), sg.symbols("t"), sg.symbols("n")]))
    middle = functools.reduce(lambda a, b: a.union(b),
            [ rnd.ExprSymbols(32, 38), rnd.ExprSymbols(40, 91), rnd.ExprSymbols(93, 126),
                escape ])
    return sg.symbols("'").concatenation(middle).concatenation(sg.symbols("'"))

@sg.token("string")
def string():
    char = functools.reduce(lambda a, b: a.union(b),
            [ rnd.ExprSymbols(32, 33), rnd.ExprSymbols(35, 91), rnd.ExprSymbols(93, 126) ])
    char = char.union(sg.symbols("\\").concatenation(rnd.ExprSymbols(32, 126)))
    string = char.closure()
    return sg.symbols('"').concatenation(string).concatenation(sg.symbols('"'))

if __name__ == "__main__":
    sg.generate(basetemp="template.cpp",
            tempdir="templates",
            outfile="output.cpp")
