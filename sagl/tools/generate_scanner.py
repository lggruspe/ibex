import scangen as sg

@sg.token("a")
def a():
    return sg.symbols('a')

@sg.token("b")
def b():
    return sg.symbols('b')

@sg.token("whitespace")
def whitespace():
    return sg.symbols(' ').union(sg.symbols('\t')).union(sg.symbols('\n'))

if __name__ == "__main__":
    sg.generate()
