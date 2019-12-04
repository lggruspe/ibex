import random
import examples

DIGITS = "0123456789"

@examples.instances
def empty():
    return ''

@examples.instances
def number():
    def integer():
        n = random.randint(1, 9)
        if n == 1:
            return random.choice(DIGITS)

        rv = random.choice(DIGITS[1:])
        for i in range(1, n):
            rv += random.choice(DIGITS)
        return rv

    rv = integer()
    if random.randint(0, 1):
        rv += "."
        for _ in range(random.randint(1, 9)):
            rv += random.choice(DIGITS)
    if random.randint(0, 1):
        rv += random.choice("eE")
        if random.randint(0, 1):
            rv += random.choice("+-")
        rv += integer()
    return rv

@examples.instances
def character():
    escape = "\\{}".format(random.choice("nt\\"))
    char = chr(random.choice([32, 38] + [40, 91] + [93, 126]))
    return "'{}'".format(random.choice([escape, char]))

@examples.instances
def string():
    char = chr(random.choice([32, 33] + [35, 91] + [93, 126]))
    escape = "\\{}".format(chr(random.choice([32, 126])))
    string = ""
    for i in range(random.randint(0, 32)):
        string += random.choice([char, escape])
    return f'"{string}"'

@examples.instances
def whitespace():
    return random.choice([' ', "\t", "\n"])

@examples.instances
def identifier():
    A = "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
    B = "0123456789"
    rv = random.choice(A)
    for _ in range(random.randint(0, 15)):
        rv += random.choice(A + B)
    return rv

@examples.instances
def dot():
    return '.'

@examples.instances
def lparen():
    return '('

@examples.instances
def rparen():
    return ')'

@examples.instances
def comma():
    return ','

@examples.instances
def star():
    return '*'

@examples.instances
def equal():
    return '='

@examples.instances
def lbrace():
    return '{'

@examples.instances
def rbrace():
    return '}'

@examples.instances
def colon():
    return ':'

@examples.instances
def lbracket():
    return '['

@examples.instances
def rbracket():
    return ']'

@examples.instances
def plus():
    return '+'

@examples.instances
def minus():
    return '-'

@examples.instances
def slash():
    return '/'

@examples.instances
def lessthan():
    return '<'

@examples.instances
def greaterthan():
    return '>'

if __name__ == "__main__":
    examples.main()
