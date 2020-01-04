import sys

class Token:
    EMPTY = "empty"
    PIPE = "pipe"
    STAR = "star"
    LPAREN = "lparen"
    RPAREN = "rparen"
    DOT = "dot"
    QUESTION = "question"
    PLUS = "plus"
    LBRACKET = "lbracket"
    RBRACKET = "rbracket"
    DASH = "dash"
    SYMBOL = "symbol"

class InputStack:
    def __init__(self, file=sys.stdin):
        self.stack = []
        self.file = file
        self.done = False

    def get(self):
        if self.stack:
            return self.stack.pop()
        if self.done:
            return ""
        a = self.file.read(1)
        if not a:
            self.done = True
        return a

    def unget(self, a):
        if not a:
            raise Exception
        self.stack.append(a)

class BaseRecognizer:
    def __init__(self, token=Token.EMPTY, accept=False, error=-1):
        self.token = token
        self.accept = accept
        self.error = error

    def next(self, q, a):
        raise NotImplementedError

    def match(self, io=InputStack()):
        checkpoint = [0]
        lexeme = []
        while checkpoint[-1] != self.error and (a := io.get()):
            status, r = self.next(checkpoint[-1], ord(a))
            if status == 1:
                self.accept = True
                checkpoint.clear()
            checkpoint.append(r)
            lexeme.append(a)
        for _ in range(len(checkpoint), 1, -1):
            io.unget(lexeme.pop())
        return self.accept, "".join(lexeme)


class Pipe(BaseRecognizer):
    def __init__(self):
        super().__init__(Token.PIPE, False, 2)

    def next(self, q, a):
        if q == 0:
            if a == 124:
                return 1, 1
            return -1, 2
        if q == 1:
            return -1, 2
        return -1, 2

class Star(BaseRecognizer):
    def __init__(self):
        super().__init__(Token.STAR, False, 2)

    def next(self, q, a):
        if q == 0:
            if a == 42:
                return 1, 1
            return -1, 2
        if q == 1:
            return -1, 2
        return -1, 2

class Lparen(BaseRecognizer):
    def __init__(self):
        super().__init__(Token.LPAREN, False, 2)

    def next(self, q, a):
        if q == 0:
            if a == 40:
                return 1, 1
            return -1, 2
        if q == 1:
            return -1, 2
        return -1, 2

class Rparen(BaseRecognizer):
    def __init__(self):
        super().__init__(Token.RPAREN, False, 2)

    def next(self, q, a):
        if q == 0:
            if a == 41:
                return 1, 1
            return -1, 2
        if q == 1:
            return -1, 2
        return -1, 2

class Dot(BaseRecognizer):
    def __init__(self):
        super().__init__(Token.DOT, False, 2)

    def next(self, q, a):
        if q == 0:
            if a == 46:
                return 1, 1
            return -1, 2
        if q == 1:
            return -1, 2
        return -1, 2

class Question(BaseRecognizer):
    def __init__(self):
        super().__init__(Token.QUESTION, False, 2)

    def next(self, q, a):
        if q == 0:
            if a == 63:
                return 1, 1
            return -1, 2
        if q == 1:
            return -1, 2
        return -1, 2

class Plus(BaseRecognizer):
    def __init__(self):
        super().__init__(Token.PLUS, False, 2)

    def next(self, q, a):
        if q == 0:
            if a == 43:
                return 1, 1
            return -1, 2
        if q == 1:
            return -1, 2
        return -1, 2

class Lbracket(BaseRecognizer):
    def __init__(self):
        super().__init__(Token.LBRACKET, False, 2)

    def next(self, q, a):
        if q == 0:
            if a == 91:
                return 1, 1
            return -1, 2
        if q == 1:
            return -1, 2
        return -1, 2

class Rbracket(BaseRecognizer):
    def __init__(self):
        super().__init__(Token.RBRACKET, False, 2)

    def next(self, q, a):
        if q == 0:
            if a == 93:
                return 1, 1
            return -1, 2
        if q == 1:
            return -1, 2
        return -1, 2

class Dash(BaseRecognizer):
    def __init__(self):
        super().__init__(Token.DASH, False, 2)

    def next(self, q, a):
        if q == 0:
            if a == 45:
                return 1, 1
            return -1, 2
        if q == 1:
            return -1, 2
        return -1, 2

class Symbol(BaseRecognizer):
    def __init__(self):
        super().__init__(Token.SYMBOL, False, 2)

    def next(self, q, a):
        if q == 0:
            if 0 <= a < 40:
                return 1, 1
            if a == 40:
                return 1, 1
            if a == 41:
                return 1, 1
            if a == 42:
                return 1, 1
            if a == 43:
                return 1, 1
            if 44 <= a < 46:
                return 1, 1
            if a == 46:
                return 1, 1
            if a == 47:
                return 1, 1
            if 48 <= a < 58:
                return 1, 1
            if 58 <= a < 63:
                return 1, 1
            if a == 63:
                return 1, 1
            if a == 64:
                return 1, 1
            if 65 <= a < 71:
                return 1, 1
            if 71 <= a < 88:
                return 1, 1
            if a == 88:
                return 1, 1
            if 89 <= a < 91:
                return 1, 1
            if a == 92:
                return 0, 3
            if 94 <= a < 97:
                return 1, 1
            if 97 <= a < 103:
                return 1, 1
            if 103 <= a < 120:
                return 1, 1
            if a == 120:
                return 1, 1
            if 121 <= a < 124:
                return 1, 1
            if a == 124:
                return 1, 1
            if 125 <= a < 4294967295:
                return 1, 1
            return -1, 2
        if q == 1:
            return -1, 2
        if q == 3:
            if a == 40:
                return 1, 1
            if a == 41:
                return 1, 1
            if a == 42:
                return 1, 1
            if a == 43:
                return 1, 1
            if a == 46:
                return 1, 1
            if a == 63:
                return 1, 1
            if a == 88:
                return 0, 4
            if a == 91:
                return 1, 1
            if a == 92:
                return 1, 1
            if a == 93:
                return 1, 1
            if a == 120:
                return 0, 4
            if a == 124:
                return 1, 1
            return -1, 2
        if q == 4:
            if 48 <= a < 58:
                return 1, 5
            if 65 <= a < 71:
                return 1, 5
            if 97 <= a < 103:
                return 1, 5
            return -1, 2
        if q == 5:
            if 48 <= a < 58:
                return 1, 5
            if 65 <= a < 71:
                return 1, 5
            if 97 <= a < 103:
                return 1, 5
            return -1, 2
        return -1, 2

def match_longest(*recs, io=InputStack()):
    token = Token.EMPTY
    lexeme = ""
    for T in recs:
        r = T()
        ok, s = r.match(io)
        if ok and len(s) > len(lexeme):
            token = r.token
            lexeme = s
        for a in s[-1::-1]:
            io.unget(a)
    for _ in lexeme:
        io.get()
    return token, lexeme

SCANNERS = {
    "pipe": Pipe,
    "star": Star,
    "lparen": Lparen,
    "rparen": Rparen,
    "dot": Dot,
    "question": Question,
    "plus": Plus,
    "lbracket": Lbracket,
    "rbracket": Rbracket,
    "dash": Dash,
    "symbol": Symbol,
}
