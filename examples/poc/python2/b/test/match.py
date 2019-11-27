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

def single_default(file, scanner_constructor, record_token="", record_lexeme=""):
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
            if len(lexeme) > len(record_lexeme):
                record_token = scanner.token
                record_lexeme = lexeme
            break
    return record_token, record_lexeme, lexeme

def fsingle(file, scanner_constructor):
    _, _, lexeme = single_default(file, scanner_constructor)
    return lexeme

def single(scanner):
    return fsingle(None, scanner)

def flongest(file, *scanners):
    if not scanners:
        return "", ""
    record_token = ""
    record_lexeme = ""
    for scanner_constructor in scanners:
        record_token, record_lexeme, _ = single_default(
            file, scanner_constructor, record_token, record_lexeme)
    return record_token, record_lexeme

def longest(*scanners):
    return flongest(None, *scanners)

def ftokenizer(file, *scanners):
    while True:
        token, lexeme = longest(*scanners)
        for _ in lexeme:
            io_get(file)
        if not token:
            break
        yield token, lexeme

def tokenizer(*scanners):
    return ftokenizer(None, *scanners)
