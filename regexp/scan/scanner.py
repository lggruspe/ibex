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
    INTERVAL = "interval"
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

class Interval(BaseRecognizer):
    def __init__(self):
        super().__init__(Token.INTERVAL, False, 10)

    def next(self, q, a):
        if q == 0:
            if a == 91:
                return 0, 6
            return -1, 10
        if q == 6:
            if 0 <= a < 40:
                return 0, 5
            if a == 40:
                return 0, 5
            if a == 41:
                return 0, 5
            if a == 42:
                return 0, 5
            if a == 43:
                return 0, 5
            if a == 44:
                return 0, 5
            if a == 45:
                return 0, 5
            if a == 46:
                return 0, 5
            if a == 47:
                return 0, 5
            if 48 <= a < 58:
                return 0, 5
            if 58 <= a < 63:
                return 0, 5
            if a == 63:
                return 0, 5
            if a == 64:
                return 0, 5
            if 65 <= a < 71:
                return 0, 5
            if 71 <= a < 91:
                return 0, 5
            if a == 92:
                return 0, 7
            if 94 <= a < 97:
                return 0, 5
            if 97 <= a < 103:
                return 0, 5
            if 103 <= a < 120:
                return 0, 5
            if a == 120:
                return 0, 5
            if 121 <= a < 124:
                return 0, 5
            if a == 124:
                return 0, 5
            if 125 <= a < 4294967295:
                return 0, 5
            return -1, 10
        if q == 1:
            return -1, 10
        if q == 2:
            if a == 93:
                return 1, 1
            return -1, 10
        if q == 3:
            if 0 <= a < 40:
                return 0, 2
            if a == 40:
                return 0, 2
            if a == 41:
                return 0, 2
            if a == 42:
                return 0, 2
            if a == 43:
                return 0, 2
            if a == 44:
                return 0, 2
            if a == 45:
                return 0, 2
            if a == 46:
                return 0, 2
            if a == 47:
                return 0, 2
            if 48 <= a < 58:
                return 0, 2
            if 58 <= a < 63:
                return 0, 2
            if a == 63:
                return 0, 2
            if a == 64:
                return 0, 2
            if 65 <= a < 71:
                return 0, 2
            if 71 <= a < 91:
                return 0, 2
            if a == 92:
                return 0, 4
            if 94 <= a < 97:
                return 0, 2
            if 97 <= a < 103:
                return 0, 2
            if 103 <= a < 120:
                return 0, 2
            if a == 120:
                return 0, 2
            if 121 <= a < 124:
                return 0, 2
            if a == 124:
                return 0, 2
            if 125 <= a < 4294967295:
                return 0, 2
            return -1, 10
        if q == 4:
            if a == 40:
                return 0, 2
            if a == 41:
                return 0, 2
            if a == 42:
                return 0, 2
            if a == 43:
                return 0, 2
            if a == 46:
                return 0, 2
            if a == 63:
                return 0, 2
            if a == 91:
                return 0, 2
            if a == 92:
                return 0, 2
            if a == 93:
                return 0, 2
            if a == 120:
                return 0, 8
            if a == 124:
                return 0, 2
            return -1, 10
        if q == 8:
            if 48 <= a < 58:
                return 0, 11
            if 65 <= a < 71:
                return 0, 11
            if 97 <= a < 103:
                return 0, 11
            return -1, 10
        if q == 5:
            if a == 45:
                return 0, 3
            return -1, 10
        if q == 7:
            if a == 40:
                return 0, 5
            if a == 41:
                return 0, 5
            if a == 42:
                return 0, 5
            if a == 43:
                return 0, 5
            if a == 46:
                return 0, 5
            if a == 63:
                return 0, 5
            if a == 91:
                return 0, 5
            if a == 92:
                return 0, 5
            if a == 93:
                return 0, 5
            if a == 120:
                return 0, 9
            if a == 124:
                return 0, 5
            return -1, 10
        if q == 9:
            if 48 <= a < 58:
                return 0, 12
            if 65 <= a < 71:
                return 0, 12
            if 97 <= a < 103:
                return 0, 12
            return -1, 10
        if q == 11:
            if 48 <= a < 58:
                return 0, 11
            if 65 <= a < 71:
                return 0, 11
            if a == 93:
                return 1, 1
            if 97 <= a < 103:
                return 0, 11
            return -1, 10
        if q == 12:
            if a == 45:
                return 0, 3
            if 48 <= a < 58:
                return 0, 12
            if 65 <= a < 71:
                return 0, 12
            if 97 <= a < 103:
                return 0, 12
            return -1, 10
        return -1, 10

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
            if 71 <= a < 91:
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
    "interval": Interval,
    "symbol": Symbol,
}
