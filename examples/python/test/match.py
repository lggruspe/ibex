import sys

INPUT_STACK = []

def io_get(file=None):
    if INPUT_STACK:
        return INPUT_STACK.pop()
    if not file:
        char = sys.stdin.read(1)
        return char
    line = file.readline()
    if not line:
        return ""
    for char in reversed(line):
        INPUT_STACK.append(char)
    return INPUT_STACK.pop()

def io_unget(char):
    INPUT_STACK.append(char)

def io_iterate(file=None):
    while True:
        char = io_get(file)
        if not char:
            break
        yield char

def fsingle(file, scanner_constructor):
    scanner = scanner_constructor()
    lexeme = ""
    for char in io_iterate(file):
        lexeme += char
        if not scanner.next(ord(char)):
            for a in reversed(lexeme):
                io_unget(a)
            steps = scanner.backtrack_steps()
            if steps > 0:
                lexeme = lexeme[:-steps]
            break
    return lexeme

def single(scanner):
    return fsingle(None, scanner)

def flongest(file, *args):
    if not args:
        return "", ""
    scanners = [scanner() for scanner in args]
    record_scanner = None
    record_lexeme = ""
    for scanner in scanners:
        lexeme = ""
        for char in io_iterate(file):
            lexeme += char
            if not scanner.next(ord(char)):
                for a in reversed(lexeme):
                    io_unget(a)
                steps = scanner.backtrack_steps()
                if steps > 0:
                    lexeme = lexeme[:-steps]
                if scanner.accepts:
                    if len(lexeme) > len(record_lexeme):
                        record_scanner = scanner
                        record_lexeme = lexeme
                break

    token = ""
    if record_lexeme:
        token = record_scanner.token
        for _ in record_lexeme:
            io_get(file)
    return token, record_lexeme

def longest(*scanners):
    return flongest(None, *scanners)

def ftokenizer(file, *scanners):
    while True:
        token, lexeme = longest(*scanners)
        if not token:
            break
        yield token, lexeme

def tokenizer(*scanners):
    return ftokenizer(None, *scanners)
