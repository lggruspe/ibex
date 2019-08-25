#!/usr/bin/env python3
from collections import deque
import sys

INPUT_STACK = []

def io_get():
    if INPUT_STACK:
        return INPUT_STACK.pop()
    char = sys.stdin.read(1)
    if not char:
        raise StopIteration
    return ord(char)

def io_unget(char):
    INPUT_STACK.append(char)

def io_iterate():
    while True:
        yield io_get()

class Scanner:
    def __init__(self, token):
        self.state = 0
        self.checkpoint = 0
        self.token = token

    def is_error(self):
        return self.state == -1

    def reset(self):
        self.state = 0

    def fail(self):
        self.state = -1
        return False

    def next(self, char):
        if not char:
            return self.fail()
        assert not self.is_error()
        try:
            transition = getattr(self, f"s{self.state}")
            transition(char)
            return True
        except AttributeError:
            return self.fail()

    def backtrack(self):
        # can backtrack one step
        self.state = self.checkpoint
        self.checkpoint = -1

    def __repr__(self):
        return f"<Scanner {self.token} state:{self.state} checkpoint:{self.checkpoint}>"

## for scanner in scanners
## include "scanner.py"
## endfor

def scan():
    """Assume each scanner starts in a valid state."""
    scanners = deque([
## for scanner in scanners
        {{ scanner.token|title }}Scanner(),
## endfor
    ])
    token = None
    lexeme = ""
    if not scanners:
        return token, lexeme

    # even if there are multiple matches, the input will eventually run out
    # when the loop breaks, scanners[0] is matched
    while len(scanners) > 1:
        try:
            char = io_get()
        except StopIteration:
            break
        lexeme += chr(char)
        scanner = scanners.popleft()
        scanner.next(char)
        if not scanner.is_error():
            scanners.append(scanner)

    # proceed scanning solo, rollback if needed
    scanner = scanners[0]
    token = scanner.token
    for char in io_iterate():
        lexeme += chr(char)
        scanner.next(char)
        if scanner.is_error():
            lexeme = lexeme[:-1]
            scanner.backtrack()
            break
    return token, lexeme

if __name__ == "__main__":
    token, lexeme = scan()
    print("Token:", token)
    print("Lexeme:", lexeme)
