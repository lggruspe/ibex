#!/usr/bin/env python3
import sys

def get_input():
    while True:
        char = sys.stdin.read(1)
        if not char:
            raise StopIteration
        yield ord(char)

class Scanner:
    def __init__(self):
        self.start = 0
        self.state = self.start

    def is_error(self):
        return self.state == -1

    def reset(self):
        self.state = self.start

    def fail(self):
        self.state = -1

    def next(self, char):
        assert not self.is_error()
        try:
            transition = getattr(self, f"s{self.state}")
            transition(char)
        except AttributeError:
            self.fail()

## for scanner in scanners
## include "scanner.py"
## endfor

def main():
    scanners = [
## for scanner in scanners
        {{ scanner.token|title }}Scanner(),
## endfor
    ]
    token = None
    lexeme = ""
    for char in get_input():
        if not scanners:
            break
        for i, scanner in enumerate(scanners):
            if not scanner.is_error():
                scanner.next(char)
            else:
                del scanners[i]

    # TODO rollback
    return token, lexeme

if __name__ == "__main__":
    _, lexeme = main()
    print(lexeme)
