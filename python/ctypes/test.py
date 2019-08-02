from rnd import Symbols, convert, Dfa
import unittest

def identifier_expr():
    underscore = Symbols(ord("_"))
    letters = Symbols(ord("a"), ord("z")).union(Symbols(ord("A"), ord("Z")))
    digits = Symbols(ord("0"), ord("9"))
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
