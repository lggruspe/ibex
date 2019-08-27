import random
import lexeme

DIGITS = "0123456789"

@lexeme.randomizer
def empty():
    return ''

@lexeme.randomizer
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

@lexeme.randomizer
def character():
    escape = "\\{}".format(random.choice("nt\\"))
    char = chr(random.choice([32, 38] + [40, 91] + [93, 126]))
    return "'{}'".format(random.choice([escape, char]))

@lexeme.randomizer
def string():
    char = chr(random.choice([32, 33] + [35, 91] + [93, 126]))
    escape = "\\{}".format(chr(random.choice([32, 126])))
    string = ""
    for i in range(random.randint(0, 32)):
        string += random.choice([char, escape])
    return f'"{string}"'

@lexeme.randomizer
def whitespace():
    return random.choice([' ', "\t", "\n"])

@lexeme.randomizer
def identifier():
    A = "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
    B = "0123456789"
    rv = random.choice(A)
    for _ in range(random.randint(0, 15)):
        rv += random.choice(A + B)
    return rv

if __name__ == "__main__":
    lexeme.main()
