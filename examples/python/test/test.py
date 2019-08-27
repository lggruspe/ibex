import os.path
import subprocess
import sys
import unittest
import examples
import match

sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), "..")))

from output.scanners import *

SCANNERS = {
    "identifier": IdentifierScanner,
    "empty": EmptyScanner,
    "whitespace": WhitespaceScanner,
    "number": NumberScanner,
    "character": CharacterScanner,
    "string": StringScanner,
}

TEST_DATA = {}

def run_longest(pinput):
    out = subprocess.check_output(
        ["python", "run.py", "longest"],
        input=f"{pinput}\n".encode())
    token, lexeme = out.decode()[:-1].split('\n', maxsplit=1)
    return token, lexeme

def parametrize_longest_match_test(test_case, scanner_type):
    for word, label in TEST_DATA[scanner_type]:
        token, lexeme = run_longest(word)
        if label:
            test_case.assertEqual(token, scanner_type)
            test_case.assertEqual(lexeme, word)
        else:
            test_case.assertNotEqual(token, scanner_type)

class LongestMatchTest(unittest.TestCase):
    @unittest.skip("")
    def test_empty(self):
        parametrize_longest_match_test(self, "empty")

    def test_except_empty(self):
        for scanner_type in SCANNERS:
            if scanner_type != "empty":
                with self.subTest(scanner_type=scanner_type):
                    parametrize_longest_match_test(self, scanner_type)

class SingleMatchTest(unittest.TestCase):
    @unittest.skip("")
    def test_empty(self):
        scanner = EmptyScanner()
        lexeme = match.single(scanner)
        self.assertEqual("", lexeme)

    @unittest.skip("")
    def test_identifier(self):
        scanner = IdentifierScanner()

    @unittest.skip("")
    def test_whitespace(self):
        scanner = WhitespaceScanner()

    @unittest.skip("")
    def test_number(self):
        scanner = NumberScanner()

    @unittest.skip("")
    def test_character(self):
        scanner = CharacterScanner()

    @unittest.skip("")
    def test_string(self):
        scanner = StringScanner()

class TokenizerTest(unittest.TestCase):
    def setUp(self):
        self.scanners = [scanner() for scanner in SCANNERS.values()]

    @unittest.skip("")
    def test_tokenize(self):
        tokenize = match.tokenizer(*(self.scanners))
        for token, lexeme in tokenize:
            pass

if __name__ == "__main__":
    for scanner_type in SCANNERS:
        with open(f"data/{scanner_type}.csv", "r") as file:
            TEST_DATA[scanner_type] = examples.read(file)
    unittest.main()
