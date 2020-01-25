import os
import sys

path = os.path.abspath(os.path.join(os.path.dirname(__file__), "../../scangen"))
sys.path.append(path)

from scangen import generate, from_class

class Scanner:
    a = 'a'
    b = 'b'
    whitespace = "[ \t\n]"

if __name__ == "__main__":
    generate(from_class(Scanner))
