from rnd import ExprSymbols, convert, Dfa, DfaSymbols
import unittest

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
        self.assertTrue(False)

if __name__ == "__main__":
    unittest.main()
