import sys
from linkedlist import LinkedList

INPUT_STACK = []

def io_get():
    if INPUT_STACK:
        return INPUT_STACK.pop()
    char = sys.stdin.read(1)
    return char

def io_unget(char):
    INPUT_STACK.append(char)

def io_iterate():
    while True:
        char = io_get()
        if not char:
            break
        yield char

def longest_match(*args):
    if not args:
        return "", ""
    scanners = LinkedList(args)
    lexeme = ""
    while len(scanners) > 1:
        char = io_get()
        if not char:
            break
        lexeme += char
        scanner = scanners.popleft()
        if scanner.next(char):
            scanners.append(scanner)

    # TODO pick leftmost scanner in args, instead of scanners[0]
    scanner = scanners[0]
    for char in io_iterate():
        lexeme += char
        if not scanner.next(char):
            io_unget(lexeme[-1])
            lexeme = lexeme[:-1]
            scanner.backtrack()
            break
    return scanner.token, lexeme
