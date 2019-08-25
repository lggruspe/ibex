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

def single(scanner, lexeme=""):
    for char in io_iterate():
        if not scanner.next(char):
            io_unget(char)
            scanner.backtrack()
            break
        lexeme += char
    if not lexeme:
        return "", ""
    return scanner.token, lexeme

def longest(*args):
    if not args:
        return "", ""
    scanners = LinkedList(args)
    lexeme = ""
    if len(scanners) > 1:
        for char in io_iterate():
            lexeme += char
            for node in scanners.iteratenodes():
                scanner = node.data
                if not scanner.next(char):
                    scanners.deletenode(node)
                if len(scanners) <= 1:
                    break
            else:
                continue
            break
    return single(scanners[0], lexeme)
