from scangen2 import generate, from_class

class Scanner:
    integer = "0|[1-9][0-9]*"

generate(from_class(Scanner))
