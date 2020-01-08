import sys

class Token:
    EMPTY = "empty"
    IDENTIFIER = "identifier"
    WHITESPACE = "whitespace"
    NUMBER = "number"
    CHARACTER = "character"
    STRING = "string"
    DOT = "dot"
    LPAREN = "lparen"
    RPAREN = "rparen"
    COMMA = "comma"
    STAR = "star"
    EQUAL = "equal"
    LBRACE = "lbrace"
    RBRACE = "rbrace"
    COLON = "colon"
    LBRACKET = "lbracket"
    RBRACKET = "rbracket"
    PLUS = "plus"
    MINUS = "minus"
    SLASH = "slash"
    LESSTHAN = "lessthan"
    GREATERTHAN = "greaterthan"

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


class Identifier(BaseRecognizer):
    def __init__(self):
        super().__init__(Token.IDENTIFIER, False, 2)

    def next(self, q, a):
        if q == 0:
            if 65 <= a < 91:
                return 1, 1
            if a == 95:
                return 1, 1
            if 97 <= a < 123:
                return 1, 1
            return -1, 2
        if q == 1:
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
    def __init__(self):
        super().__init__(Token.WHITESPACE, False, 2)

    def next(self, q, a):
        if q == 0:
            if a == 9:
                return 1, 1
            if a == 10:
                return 1, 1
            if a == 32:
                return 1, 1
            return -1, 2
        if q == 1:
            return -1, 2
        return -1, 2

class Number(BaseRecognizer):
    def __init__(self):
        super().__init__(Token.NUMBER, False, 6)

    def next(self, q, a):
        if q == 0:
            if a == 48:
                return 1, 3
            if 49 <= a < 58:
                return 1, 8
            return -1, 6
        if q == 1:
            return -1, 6
        if q == 2:
            if a == 48:
                return 1, 1
            if 49 <= a < 58:
                return 1, 4
            return -1, 6
        if q == 3:
            if a == 46:
                return 0, 7
            if a == 69:
                return 0, 5
            if a == 101:
                return 0, 5
            return -1, 6
        if q == 4:
            if a == 48:
                return 1, 4
            if 49 <= a < 58:
                return 1, 4
            return -1, 6
        if q == 5:
            if a == 43:
                return 0, 2
            if a == 45:
                return 0, 2
            if a == 48:
                return 1, 1
            if 49 <= a < 58:
                return 1, 4
            return -1, 6
        if q == 7:
            if a == 48:
                return 1, 9
            if 49 <= a < 58:
                return 1, 9
            return -1, 6
        if q == 8:
            if a == 46:
                return 0, 7
            if a == 48:
                return 1, 8
            if 49 <= a < 58:
                return 1, 8
            if a == 69:
                return 0, 5
            if a == 101:
                return 0, 5
            return -1, 6
        if q == 9:
            if a == 48:
                return 1, 9
            if 49 <= a < 58:
                return 1, 9
            if a == 69:
                return 0, 5
            if a == 101:
                return 0, 5
            return -1, 6
        return -1, 6

class Character(BaseRecognizer):
    def __init__(self):
        super().__init__(Token.CHARACTER, False, 5)

    def next(self, q, a):
        if q == 0:
            if a == 39:
                return 0, 3
            return -1, 5
        if q == 1:
            return -1, 5
        if q == 2:
            if a == 39:
                return 1, 1
            return -1, 5
        if q == 3:
            if 32 <= a < 39:
                return 0, 2
            if 40 <= a < 92:
                return 0, 2
            if a == 92:
                return 0, 4
            if 93 <= a < 110:
                return 0, 2
            if a == 110:
                return 0, 2
            if 111 <= a < 116:
                return 0, 2
            if a == 116:
                return 0, 2
            if 117 <= a < 127:
                return 0, 2
            return -1, 5
        if q == 4:
            if a == 39:
                return 0, 2
            if a == 92:
                return 0, 2
            if a == 110:
                return 0, 2
            if a == 116:
                return 0, 2
            return -1, 5
        return -1, 5

class String(BaseRecognizer):
    def __init__(self):
        super().__init__(Token.STRING, False, 4)

    def next(self, q, a):
        if q == 0:
            if a == 34:
                return 0, 2
            return -1, 4
        if q == 1:
            return -1, 4
        if q == 2:
            if 32 <= a < 34:
                return 0, 2
            if a == 34:
                return 1, 1
            if 35 <= a < 92:
                return 0, 2
            if a == 92:
                return 0, 3
            if 93 <= a < 127:
                return 0, 2
            return -1, 4
        if q == 3:
            if 32 <= a < 34:
                return 0, 2
            if a == 34:
                return 0, 2
            if 35 <= a < 92:
                return 0, 2
            if a == 92:
                return 0, 2
            if 93 <= a < 127:
                return 0, 2
            return -1, 4
        return -1, 4

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

class Comma(BaseRecognizer):
    def __init__(self):
        super().__init__(Token.COMMA, False, 2)

    def next(self, q, a):
        if q == 0:
            if a == 44:
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

class Equal(BaseRecognizer):
    def __init__(self):
        super().__init__(Token.EQUAL, False, 2)

    def next(self, q, a):
        if q == 0:
            if a == 61:
                return 1, 1
            return -1, 2
        if q == 1:
            return -1, 2
        return -1, 2

class Lbrace(BaseRecognizer):
    def __init__(self):
        super().__init__(Token.LBRACE, False, 2)

    def next(self, q, a):
        if q == 0:
            if a == 123:
                return 1, 1
            return -1, 2
        if q == 1:
            return -1, 2
        return -1, 2

class Rbrace(BaseRecognizer):
    def __init__(self):
        super().__init__(Token.RBRACE, False, 2)

    def next(self, q, a):
        if q == 0:
            if a == 125:
                return 1, 1
            return -1, 2
        if q == 1:
            return -1, 2
        return -1, 2

class Colon(BaseRecognizer):
    def __init__(self):
        super().__init__(Token.COLON, False, 2)

    def next(self, q, a):
        if q == 0:
            if a == 58:
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

class Minus(BaseRecognizer):
    def __init__(self):
        super().__init__(Token.MINUS, False, 2)

    def next(self, q, a):
        if q == 0:
            if a == 45:
                return 1, 1
            return -1, 2
        if q == 1:
            return -1, 2
        return -1, 2

class Slash(BaseRecognizer):
    def __init__(self):
        super().__init__(Token.SLASH, False, 2)

    def next(self, q, a):
        if q == 0:
            if a == 47:
                return 1, 1
            return -1, 2
        if q == 1:
            return -1, 2
        return -1, 2

class Lessthan(BaseRecognizer):
    def __init__(self):
        super().__init__(Token.LESSTHAN, False, 2)

    def next(self, q, a):
        if q == 0:
            if a == 60:
                return 1, 1
            return -1, 2
        if q == 1:
            return -1, 2
        return -1, 2

class Greaterthan(BaseRecognizer):
    def __init__(self):
        super().__init__(Token.GREATERTHAN, False, 2)

    def next(self, q, a):
        if q == 0:
            if a == 62:
                return 1, 1
            return -1, 2
        if q == 1:
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
    "identifier": Identifier,
    "whitespace": Whitespace,
    "number": Number,
    "character": Character,
    "string": String,
    "dot": Dot,
    "lparen": Lparen,
    "rparen": Rparen,
    "comma": Comma,
    "star": Star,
    "equal": Equal,
    "lbrace": Lbrace,
    "rbrace": Rbrace,
    "colon": Colon,
    "lbracket": Lbracket,
    "rbracket": Rbracket,
    "plus": Plus,
    "minus": Minus,
    "slash": Slash,
    "lessthan": Lessthan,
    "greaterthan": Greaterthan,
}