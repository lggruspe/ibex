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

'''
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
'''

def longest(*args):
    if not args:
        return "", ""
    scanners = args[:]
    record_scanner = None
    record_lexeme = ""
    for scanner in scanners:
        lexeme = ""
        for char in io_iterate():
            lexeme += char
            if not scanner.next(char):
                for a in reversed(lexeme):
                    io_unget(a)
                steps = scanner.backtrack()
                if scanner.accepts():
                    if steps > 0:
                        lexeme = lexeme[:-steps]
                    if len(lexeme) > len(record_lexeme):
                        record_scanner = scanner
                        record_lexeme = lexeme
                break
    token = record_scanner.token if record_scanner else None
    return token, record_lexeme

def tokenize(*scanners):
    while True:
        token, lexeme = longest(*scanners)
        if not token:
            break
        yield token, lexeme
