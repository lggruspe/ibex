import os.path
import sys
import match

sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), "..")))

from output.scanners import *

scanners = {
    "empty": EmptyScanner(),
    "identifier": IdentifierScanner(),
    "whitespace": WhitespaceScanner(),
    "integer": IntegerScanner(),
    "number": NumberScanner(),
    "character": CharacterScanner(),
    "string": StringScanner(),
}

def test_longest_match():
    token, lexeme = match.longest(*(scanners.values()))
    print(token, f"\"{lexeme}\"")

def test_single_match(scanner):
    token, lexeme = match.single(scanner)
    print(token, f"\"{lexeme}\"")

def main():
    if len(sys.argv) == 1:
        return test_longest_match()
    test_single_match(scanners[sys.argv[1]])

if __name__ == "__main__":
    try:
        main()
    except KeyError:
        print("Invalid argument")
    except IndexError:
        print("Missing arguments")


