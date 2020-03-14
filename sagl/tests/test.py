import subprocess
import unittest

def run_sagl(terminal_input):
    out = subprocess.check_output(["../build/main"], input=terminal_input.encode())
    return out

def test_sagl_output(out, success, counter=None):
    if counter is None:
        counter = success
    return f"success = {success}\ncounter = {counter}\n".encode()

class BasicRegressionTest(unittest.TestCase):
    def test_a6(self):
        out = run_sagl("a"*6)
        self.assertTrue(test_sagl_output(out, 11))

    def test_a10(self):
        out = run_sagl("a"*10)
        self.assertTrue(test_sagl_output(out, 19))

    def test_asdf(self):
        out = run_sagl("asdf")
        self.assertTrue(test_sagl_output(out, 1))

if __name__ == "__main__":
    unittest.main()
