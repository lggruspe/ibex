import os.path
import sys
import unittest
import match

sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), "..")))

from output.scanners import *

class LongestMatchTest(unittest.TestCase):
    def setUp(self):
        self.scanners = [
            EmptyScanner(),
            IdentifierScanner(),
            WhitespaceScanner(),
            IntegerScanner(),
            NumberScanner(),
            CharacterScanner(),
            StringScanner(),
        ]

    def test_empty(self):
        token, lexeme = match.longest(*(self.scanners))
        self.assertEqual(token, "empty")
        self.assertEqual(lexeme, "")

    def test_identifier(self):
        token, lexeme = match.longest(*(self.scanners))
        self.assertEqual(token, "identifier")

    def test_whitespace(self):
        token, lexeme = match.longest(*(self.scanners))
        self.assertEqual(token, "whitespace")

    def test_integer(self):
        token, lexeme = match.longest(*(self.scanners))
        self.assertEqual(token, "integer")

    def test_number(self):
        token, lexeme = match.longest(*(self.scanners))
        self.assertEqual(token, "number")

    def test_character(self):
        token, lexeme = match.longest(*(self.scanners))
        self.assertEqual(token, "character")

    def test_string(self):
        token, lexeme = match.longest(*(self.scanners))
        self.assertEqual(token, "string")

class SingleMatchTest(unittest.TestCase):
    def test_empty(self):
        scanner = EmptyScanner()
        lexeme = match.single(scanner)
        self.assertEqual("", lexeme)

    def test_identifier(self):
        scanner = IdentifierScanner()

    def test_whitespace(self):
        scanner = WhitespaceScanner()

    def test_integer(self):
        scanner = IntegerScanner()

    def test_number(self):
        scanner = NumberScanner()

    def test_character(self):
        scanner = CharacterScanner()

    def test_string(self):
        scanner = StringScanner()

class TokenizerTest(unittest.TestCase):
    def setUp(self):
        self.scanners = [
            EmptyScanner(),
            IdentifierScanner(),
            WhitespaceScanner(),
            IntegerScanner(),
            NumberScanner(),
            CharacterScanner(),
            StringScanner(),
        ]

    def tearDown(self):
        pass

    def test_tokenize(self):
        tokenize = match.tokenizer(*(self.scanners))
        for token, lexeme in tokenize:
            pass

if __name__ == "__main__":
    unittest.main()
