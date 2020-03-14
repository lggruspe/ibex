import os.path as op
import sys

sys.path.append(op.abspath(op.join(op.dirname(__file__), "../../../scangen")))

from scangen import render, from_class

class Scanner:
    a = 'a'
    b = 'b'
    whitespace = "[ \t\n]"

config = None

def main():
    code = render(from_class(Scanner), "template.cpp", config=config)
    relpath = "../../src/scanner.hpp"
    abspath = op.abspath(op.join(op.dirname(__file__), relpath))
    with open(abspath, "w") as f:
        print(code, file=f)

if __name__ == "__main__":
    main()
