import csv
import functools
import unittest
from rnd import ExprSymbols, convert, Dfa, DfaSymbols
from rnd.internals import crnd

TEST_DATA = {
    "empty": [],
    "number": [],
    "identifier": [],
    "string": [],
    "character": [],
    "whitespace": [],
}

def sym(a, b=None):
    if b is None:
        b = a
    assert len(a) == 1 and len(b) == 1
    a, b = ord(a), ord(b)
    assert a <= b
    return ExprSymbols(a, b)

def optional(expr):
    return expr.union(sym('\0'))

def to_dfa(expr):
    """Converts expr to dfa and destroys expr."""
    dfa = convert(expr)
    expr.destroy()
    return dfa

def empty_expr():
    return sym('\0')

def identifier_expr():
    letters = sym('_').union(sym('a', 'z')).union(sym('A', 'Z'))
    return letters.concatenation(letters.union(sym('0', '9')).closure())

def integer_expr():
    return sym('0').union(sym('1', '9').concatenation(sym('0', '9').closure()))

def whitespace_expr():
    return sym(' ').union(sym('\t')).union(sym('\n'))

def number_expr():
    digit = sym('0', '9')
    decimal = sym('.').concatenation(digit).concatenation(digit.closure())
    sign = optional(sym('-').union(sym('+')))
    exponent = (sym('e').union(sym('E'))).concatenation(sign).concatenation(integer_expr())
    return integer_expr().concatenation(optional(decimal)).concatenation(optional(exponent))

def string_expr():
    char = functools.reduce(lambda a, b: a.union(b),
            [ ExprSymbols(32, 33), ExprSymbols(35, 91), ExprSymbols(93, 126) ])
    char = char.union(sym("\\").concatenation(ExprSymbols(32, 126)))
    string = char.closure()
    return sym('"').concatenation(string).concatenation(sym('"'))

def character_expr():
    escape = sym("\\").concatenation(functools.reduce(lambda a, b: a.union(b),
        [sym("'"), sym("\\"), sym("t"), sym("n")]))
    middle = functools.reduce(lambda a, b: a.union(b),
            [ ExprSymbols(32, 38), ExprSymbols(40, 91), ExprSymbols(93, 126),
                escape ])
    return sym("'").concatenation(middle).concatenation(sym("'"))

EXPR_FN = {
    "empty": empty_expr,
    "number": number_expr,
    "identifier": identifier_expr,
    "string": string_expr,
    "character": character_expr,
    "whitespace": whitespace_expr,
}

def parametrize_expr_test(test_case, expr_type):
    test_case.dfa = to_dfa(EXPR_FN[expr_type]())
    test_case.assertTrue(TEST_DATA[expr_type])
    for word, label in TEST_DATA[expr_type]:
        word = word.encode().decode()
        accepted = test_case.dfa.compute(map(ord, word))
        test_case.assertFalse(accepted ^ label)

class RndConversionTest(unittest.TestCase):
    def setUp(self):
        self.dfa = Dfa()

    def tearDown(self):
        self.dfa = Dfa()

    def test_identifier(self):
        parametrize_expr_test(self, "identifier")

    def test_empty(self):
        parametrize_expr_test(self, "empty")

    def test_whitespace(self):
        self.dfa = to_dfa(whitespace_expr())
        self.assertTrue(self.dfa.compute(map(ord, "\t")))
        self.assertTrue(self.dfa.compute(map(ord, "\n")))
        self.assertTrue(self.dfa.compute(map(ord, " ")))
        self.assertFalse(self.dfa.compute(map(ord, "")))
        self.assertFalse(self.dfa.compute(map(ord, "  ")))

    def test_number(self):
        parametrize_expr_test(self, "number")

    def test_string(self):
        parametrize_expr_test(self, "string")

    def test_character(self):
        parametrize_expr_test(self, "character")

    def test_leaks(self):
        to_dfa(empty_expr())
        to_dfa(number_expr())
        to_dfa(identifier_expr())
        to_dfa(integer_expr())
        to_dfa(whitespace_expr())
        to_dfa(character_expr())
        to_dfa(string_expr())
        self.assertEqual(0, crnd.rnd_get_expr_counter())

def init_data(filename, seq):
    with open(filename, "r") as f:
        seq.extend([(str(row[0]), int(row[1])) for row in csv.reader(f)])

if __name__ == "__main__":
    init_data("testdata/identifier.csv", TEST_DATA["identifier"])
    init_data("testdata/number.csv", TEST_DATA["number"])
    init_data("testdata/empty.csv", TEST_DATA["empty"])
    init_data("testdata/string.csv", TEST_DATA["string"])
    init_data("testdata/character.csv", TEST_DATA["character"])
    unittest.main()
