import os.path
import subprocess
import sys
import unittest
import examples
import lexeme as lex

sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), "..")))

import output.scanners as scanners

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

def run_tokenizer(pinput):
    """Wrapper for running python run.py tokenizer."""
    out = subprocess.check_output(
        ["python", "run.py", "tokenizer"],
        input=f"{pinput}\n".encode())
    return out.decode()

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

class MatchTest(unittest.TestCase):
    def test_longest(self):
        for scanner_type in scanners.SCANNERS:
            with self.subTest(scanner_type=scanner_type):
                parametrize_longest_match_test(self, scanner_type)

    def test_single(self):
        for scanner_type in scanners.SCANNERS:
            with self.subTest(scanner_type=scanner_type):
                parametrize_single_match_test(self, scanner_type)

    def test_tokenize(self):
        identifier = lex.identifier()
        number = lex.number()
        character = lex.character()
        string = lex.string()
        expected = "".join([
            f"identifier {identifier}\n",
            f"whitespace  \n",
            f"number {number}\n",
            f"whitespace \t\n",
            f"character {character}\n",
            f"whitespace \n\n",
            f"string {string}\n",
        ])
        actual = run_tokenizer(f"{identifier} {number}\t{character}\n{string}")
        self.assertEqual(actual, expected)

if __name__ == "__main__":
    for scanner_type in scanners.SCANNERS:
        with open(f"data/{scanner_type}.csv", "r") as file:
            TEST_DATA[scanner_type] = examples.read(file)
    unittest.main()
