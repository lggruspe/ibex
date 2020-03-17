from os.path import abspath, dirname, join
import sys

BASEDIR = abspath(dirname(__file__))
sys.path.append(abspath(join(dirname(__file__), "../../../codegen")))
sys.path.append(abspath(join(dirname(__file__), "../../../rnd/python")))

from codegen import render
from rnd import convert, from_class

class Scanner:
    a = 'a'
    b = 'b'
    whitespace = "[ \t\n]"

def main():
    code = render("template.cpp", context={
        "scanner": convert(from_class(Scanner)),
        "config": None,
    })
    relpath = "../../src/examples/scanner.hpp"
    with open(abspath(join(dirname(__file__), relpath)), "w") as f:
        print(code, file=f)

if __name__ == "__main__":
    main()
