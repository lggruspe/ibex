import enum
import sys

class Token(enum.Enum):
    EMPTY = 0   # or enum.auto()?
    IDENTIFIER = enum.auto()
    WHITESPACE = enum.auto()

class InputStack:
    def __init__(self, file=sys.stdin):
        self.stack = []
        self.file = file

    def get(self):
        if not self.stack:
            return self.file.read(1)
        return self.stack.pop()

    def unget(self, a):
        if not a:
            self.stack.append(a)

class BaseRecognizer:
    def __init__(self, token=Token.EMPTY, accept=False, error=-1,
            io=InputStack()):
        self.token = token
        self.accept = accept    # should be True if 0 is an accept state
        self.error = error      # -1 if none
        self.io = InputStack()

    def next(self, q, a):
        raise NotImplementedError

    def match(self):
        checkpoint = [0]
        lexeme = []
        while checkpoint[-1] != self.error:
            a = self.io.get()
            if not a:
                break
            status, r = self.next(checkpoint[-1], ord(a))
            if status == 1:
                self.accept = True
                checkpoint.clear()
            checkpoint.append(r)
            lexeme.append(a)
        for _ in checkpoint[-1:0:-1]:
            self.io.unget(lexeme.pop())
        return self.accept, "".join(lexeme)

class Identifier(BaseRecognizer):
    def __init__(self, io=InputStack()):
        super().__init__(Token.IDENTIFIER, False, 2, io)

    def next(self, q, a):
        if q == 0:
            if 65 <= a < 91:
                return 1, 1
            if a == 95:
                return 1, 1
            if 97 <= a < 123:
                return 1, 1
            return -1, 2
        elif q == 1:
            if 48 <= a < 58:
                return 1, 1
            if 65 <= a < 91:
                return 1, 1
            if a == 95:
                return 1, 1
            if 97 <= a < 123:
                return 1, 1
            return -1, 2
        return -1, 2

class Whitespace(BaseRecognizer):
    def __init__(self, io=InputStack()):
        super().__init__(Token.WHITESPACE, False, 2, io)

    def next(self, q, a):
        if q == 0:
            if a in [9, 10, 32]:
                return 1, 1
        return -1, 2

def match_first(*recs, io=InputStack()):
    for T in recs:
        r = T(io)
        ok, s = r.match()
        if ok:
            return r.token, s
    return Token.EMPTY, ""

def match_longest(*recs, io=InputStack()):
    token = Token.EMPTY
    lexeme = ""
    for T in recs:
        r = T(io)
        ok, s = r.match()
        if ok and len(s) > len(lexeme):
            token = r.token
            lexeme = s
        for a in s[-1::-1]:
            io.unget(a)
    return token, lexeme
