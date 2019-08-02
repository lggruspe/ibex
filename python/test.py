from rnd import ExprSymbols, convert, Dfa, DfaSymbols
import unittest
import csv

identifier_data = []

def identifier_expr():
    underscore = ExprSymbols(ord("_"))
    letters = ExprSymbols(ord("a"), ord("z")).union(ExprSymbols(ord("A"), ord("Z")))
    digits = ExprSymbols(ord("0"), ord("9"))
    front = underscore.union(letters)
    middle = front.union(digits).closure()
    return front.concatenation(middle)

def identifier_dfa():
    expr = identifier_expr()
    dfa = convert(expr)
    #TODO expr.__exit__()
    return dfa

class RndConversionTest(unittest.TestCase):
    def setUp(self):
        self.dfa = identifier_dfa()

    def test_convert(self):
        self.assertTrue(identifier_data)
        for word, label in identifier_data:
            accepted = self.dfa.compute(map(ord, word))
            self.assertFalse(accepted ^ label)

if __name__ == "__main__":
    with open("testdata/identifier.csv", "r") as f:
        for row in csv.reader(f):
            word = row[0]
            label = int(row[1])
            identifier_data.append((word, label))
    unittest.main()
