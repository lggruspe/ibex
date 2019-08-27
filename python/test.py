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

    def test_leaks(self):
        for expr_fn in EXPR_FN.values():
            to_dfa(expr_fn())
        self.assertEqual(0, crnd.rnd_get_expr_counter())

    def test_random_data(self):
        for expr_type in EXPR_FN:
            with self.subTest(expr_type=expr_type):
                parametrize_expr_test(self, expr_type)

if __name__ == "__main__":
    for expr_type in EXPR_FN:
        with open(f"testdata/{expr_type}.csv", "r") as file:
            TEST_DATA[expr_type].extend([(str(row[0]), int(row[1])) for row in csv.reader(file)])
    unittest.main()
