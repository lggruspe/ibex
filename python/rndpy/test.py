from rnd import ExprSymbols, convert, Dfa, DfaSymbols, _rnd_get_expr_counter
import csv
import functools
import unittest

empty_data = []
number_data = []
identifier_data = []
string_data = []
character_data = []

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

class RndConversionTest(unittest.TestCase):
    def setUp(self):
        self.dfa = Dfa()

    def teardown(self):
        self.dfa = Dfa()

    def test_identifier(self):
        self.dfa = to_dfa(identifier_expr())
        self.assertTrue(identifier_data)
        for word, label in identifier_data:
            word = word.encode().decode(encoding="unicode-escape")
            accepted = self.dfa.compute(map(ord, word))
            self.assertFalse(accepted ^ label)

    def test_empty(self):
        self.dfa = to_dfa(empty_expr())
        self.assertTrue(empty_data)
        for word, label in empty_data:
            word = word.encode().decode(encoding="unicode-escape")
            accepted = self.dfa.compute(map(ord, word))
            self.assertFalse(accepted ^ label)

    def test_whitespace(self):
        self.dfa = to_dfa(whitespace_expr())

        self.assertTrue(self.dfa.compute(map(ord, "\t")))
        self.assertTrue(self.dfa.compute(map(ord, "\n")))
        self.assertTrue(self.dfa.compute(map(ord, " ")))
        self.assertFalse(self.dfa.compute(map(ord, "")))
        self.assertFalse(self.dfa.compute(map(ord, "  ")))

    def test_number(self):
        self.dfa = to_dfa(number_expr())
        self.assertTrue(number_data)
        for word, label in number_data:
            word = word.encode().decode(encoding="unicode-escape")
            accepted = self.dfa.compute(map(ord, word))
            self.assertFalse(accepted ^ label)

    @unittest.skip("")
    def test_string(self):
        self.dfa = to_dfa(string_expr())
        self.assertTrue(string_data)
        for word, label in string_data:
            word = word.encode().decode(encoding="unicode-escape")
            accepted = self.dfa.compute(map(ord, word))
            self.assertFalse(accepted ^ label)

    @unittest.skip("")
    def test_character(self):
        self.dfa = to_dfa(character_expr())
        self.assertTrue(character_data)
        for word, label in character_data:
            word = word.encode().decode(encoding="unicode-escape")
            accepted = self.dfa.compute(map(ord, word))
            self.assertFalse(accepted ^ False)

    def test_leaks(self):
        to_dfa(empty_expr())
        to_dfa(number_expr())
        to_dfa(identifier_expr())
        to_dfa(integer_expr())
        to_dfa(whitespace_expr())
        to_dfa(character_expr())
        to_dfa(string_expr())
        self.assertEqual(0, _rnd_get_expr_counter())

def init_data(filename, seq):
    with open(filename, "r") as f:
        seq.extend([(str(row[0]), int(row[1])) for row in csv.reader(f)])

if __name__ == "__main__":
    init_data("testdata/identifier.csv", identifier_data)
    init_data("testdata/number.csv", number_data)
    init_data("testdata/empty.csv", empty_data)
    init_data("testdata/string.csv", string_data)
    init_data("testdata/character.csv", character_data)
    unittest.main()
