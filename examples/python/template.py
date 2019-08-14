import sys

def get_input():
    while True:
        char = sys.stdin.read(1)
        if not char:
            raise StopIteration
        yield char

class Scanner:
    def __init__(self):
        self.start = 0
        self.state = self.start

    def is_error(self):
        return self.state == -1

    def reset(self):
        self.state = self.start

    def next(self, char):
        #self.state =
        raise NotImplemented

def main():
    scanners = [Scanner()]
    token = None
    lexeme = ""
    for char in get_input():
        for scanner in scanners:
            if not scanner.is_error():
                pass

    ## rollback
    return token, lexeme

if __name__ == "__main__":
    main()
