from rnd import ExprSymbols, convert, Dfa, DfaSymbols
import unittest
import csv

empty_data = []
whitespace_data = []
number_data = []
identifier_data = []

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

class RndConversionTest(unittest.TestCase):
    def setUp(self):
        self.dfa = Dfa()

    def teardown(self):
        self.dfa = Dfa()

    def test_identifier(self):
        self.dfa = to_dfa(identifier_expr())
        self.assertTrue(identifier_data)
        for word, label in identifier_data:
            accepted = self.dfa.compute(map(ord, word))
            self.assertFalse(accepted ^ label)

    def test_empty(self):
        self.dfa = to_dfa(empty_expr())
        self.assertTrue(empty_data)
        for word, label in empty_data:
            accepted = self.dfa.compute(map(ord, word))
            self.assertFalse(accepted ^ label)

    def test_whitespace(self):
        self.dfa = to_dfa(whitespace_expr())
        self.assertTrue(whitespace_data)
        for word, label in whitespace_data:
            accepted = self.dfa.compute(map(ord, word))
            self.assertFalse(accepted ^ label)

    def test_number(self):
        self.dfa = to_dfa(number_expr())
        self.assertTrue(number_data)
        for word, label in number_data:
            accepted = self.dfa.compute(map(ord, word))
            self.assertFalse(accepted ^ label)

def init_data(filename, seq):
    with open(filename, "r") as f:
        seq.extend([(str(row[0]), int(row[1])) for row in csv.reader(f)])

if __name__ == "__main__":
    init_data("testdata/identifier.csv", identifier_data)
    init_data("testdata/number.csv", number_data)
    init_data("testdata/whitespace.csv", whitespace_data)
    init_data("testdata/empty.csv", empty_data)
    unittest.main()
