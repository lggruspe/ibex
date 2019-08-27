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

def single(scanner):
    lexeme = ""
    for char in io_iterate():
        lexeme += char
        if not scanner.next(char):
            for a in reversed(lexeme):
                io_unget(a)
            steps = scanner.backtrack()
            if steps > 0:
                lexeme = lexeme[:-steps]
            break
    return lexeme

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
                if steps > 0:
                    lexeme = lexeme[:-steps]
                if scanner.accepts():
                    if len(lexeme) > len(record_lexeme):
                        record_scanner = scanner
                        record_lexeme = lexeme
                break

    token = ""
    if record_lexeme:
        token = record_scanner.token
        for _ in record_lexeme:
            io_get()
    return token, record_lexeme

def tokenizer(*scanners):
    while True:
        token, lexeme = longest(*scanners)
        if not token:
            break
        yield token, lexeme

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
    lexeme = single(scanners[0], lexeme)
    if not lexeme:
        return "", ""
    return scanner.token, lexeme
'''
