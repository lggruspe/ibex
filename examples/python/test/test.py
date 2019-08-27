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
    "whitespace": WhitespaceScanner,
    "number": NumberScanner,
    "character": CharacterScanner,
    "string": StringScanner,
}

TEST_DATA = {}

def run_longest(pinput):
    """Wrapper for running python run.py longest."""
    out = subprocess.check_output(
        ["python", "run.py", "longest"],
        input=f"{pinput}\n".encode())
    token, lexeme = out.decode()[:-1].split('\n', maxsplit=1)
    return token, lexeme

def run_single(scanner_type, pinput):
    """Wrapper for running python run.py single -s {scanner_type}."""
    out = subprocess.check_output(
        ["python", "run.py", "single", "-s", scanner_type],
        input=f"{pinput}\n".encode())
    return out.decode()[:-1]

def parametrize_longest_match_test(test_case, scanner_type):
    for word, label in TEST_DATA[scanner_type]:
        token, lexeme = run_longest(word)
        if label:
            test_case.assertEqual(token, scanner_type)
            test_case.assertEqual(lexeme, word)
        else:
            test_case.assertNotEqual(token, scanner_type)

def parametrize_single_match_test(test_case, scanner_type):
    for word, label in TEST_DATA[scanner_type]:
        lexeme = run_single(scanner_type, word)
        test_case.assertEqual(lexeme, word if label else "")

class LongestMatchTest(unittest.TestCase):
    def test_random_data(self):
        for scanner_type in SCANNERS:
            with self.subTest(scanner_type=scanner_type):
                parametrize_longest_match_test(self, scanner_type)

class SingleMatchTest(unittest.TestCase):
    @unittest.skip("")
    def test_whitespace(self):
        parametrize_single_match_test(self, "whitespace")

    def test_except_whitespace(self):
        for scanner_type in SCANNERS:
            if scanner_type != "whitespace":
                with self.subTest(scanner_type=scanner_type):
                    parametrize_single_match_test(self, scanner_type)

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
