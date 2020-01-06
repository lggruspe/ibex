from scangen import generate, from_class

class Scanner:
    a = 'a'
    b = 'b'
    whitespace = "[ \t\n]"

if __name__ == "__main__":
    generate(from_class(Scanner))
