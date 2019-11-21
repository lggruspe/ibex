import enum
import sys

class Token(enum.Enum):
    EMPTY = 0
    IDENTIFIER = enum.auto()
    WHITESPACE = enum.auto()
    NUMBER = enum.auto()
    CHARACTER = enum.auto()
    STRING = enum.auto()
    DOT = enum.auto()
    LPAREN = enum.auto()
    RPAREN = enum.auto()
    COMMA = enum.auto()
    STAR = enum.auto()
    EQUAL = enum.auto()
    LBRACE = enum.auto()
    RBRACE = enum.auto()
    COLON = enum.auto()
    LBRACKET = enum.auto()
    RBRACKET = enum.auto()
    PLUS = enum.auto()
    MINUS = enum.auto()
    SLASH = enum.auto()
    LESSTHAN = enum.auto()
    GREATERTHAN = enum.auto()

class InputStack:
    def __init__(self, file=sys.stdin):
        self.stack = []
        self.file = file

    def get(self):
        if self.stack:
            return self.stack.pop()
        return self.file.read(1)

    def unget(self, a):
        if a:
            self.stack.append(a)

class BaseRecognizer:
    def __init__(self, token=Token.EMPTY, accept=False, error=-1,
            io=InputStack()):
        self.token = token
        self.accept = accept
        self.error = error
        self.io = io

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
            if 0 <= a < 48:
                return -1, 2
            if 48 <= a < 58:
                return -1, 2
            if 58 <= a < 65:
                return -1, 2
            if 65 <= a < 91:
                return 1, 1
            if 91 <= a < 95:
                return -1, 2
            if 95 <= a < 96:
                return 1, 1
            if 96 <= a < 97:
                return -1, 2
            if 97 <= a < 123:
                return 1, 1
            if 123 <= a < 4294967295:
                return -1, 2
        if q == 1:
            if 0 <= a < 48:
                return -1, 2
            if 48 <= a < 58:
                return 1, 1
            if 58 <= a < 65:
                return -1, 2
            if 65 <= a < 91:
                return 1, 1
            if 91 <= a < 95:
                return -1, 2
            if 95 <= a < 96:
                return 1, 1
            if 96 <= a < 97:
                return -1, 2
            if 97 <= a < 123:
                return 1, 1
            if 123 <= a < 4294967295:
                return -1, 2
        return -1, 2

class Whitespace(BaseRecognizer):
    def __init__(self, io=InputStack()):
        super().__init__(Token.WHITESPACE, False, 2, io)

    def next(self, q, a):
        if q == 0:
            if 0 <= a < 9:
                return -1, 2
            if 9 <= a < 10:
                return 1, 1
            if 10 <= a < 11:
                return 1, 1
            if 11 <= a < 32:
                return -1, 2
            if 32 <= a < 33:
                return 1, 1
            if 33 <= a < 4294967295:
                return -1, 2
        if q == 1:
            if 0 <= a < 9:
                return -1, 2
            if 9 <= a < 10:
                return -1, 2
            if 10 <= a < 11:
                return -1, 2
            if 11 <= a < 32:
                return -1, 2
            if 32 <= a < 33:
                return -1, 2
            if 33 <= a < 4294967295:
                return -1, 2
        return -1, 2

class Number(BaseRecognizer):
    def __init__(self, io=InputStack()):
        super().__init__(Token.NUMBER, False, 6, io)

    def next(self, q, a):
        if q == 0:
            if 0 <= a < 43:
                return -1, 6
            if 43 <= a < 44:
                return -1, 6
            if 44 <= a < 45:
                return -1, 6
            if 45 <= a < 46:
                return -1, 6
            if 46 <= a < 47:
                return -1, 6
            if 47 <= a < 48:
                return -1, 6
            if 48 <= a < 49:
                return 1, 3
            if 49 <= a < 58:
                return 1, 8
            if 58 <= a < 69:
                return -1, 6
            if 69 <= a < 70:
                return -1, 6
            if 70 <= a < 101:
                return -1, 6
            if 101 <= a < 102:
                return -1, 6
            if 102 <= a < 4294967295:
                return -1, 6
        if q == 3:
            if 0 <= a < 43:
                return -1, 6
            if 43 <= a < 44:
                return -1, 6
            if 44 <= a < 45:
                return -1, 6
            if 45 <= a < 46:
                return -1, 6
            if 46 <= a < 47:
                return 0, 7
            if 47 <= a < 48:
                return -1, 6
            if 48 <= a < 49:
                return -1, 6
            if 49 <= a < 58:
                return -1, 6
            if 58 <= a < 69:
                return -1, 6
            if 69 <= a < 70:
                return 0, 5
            if 70 <= a < 101:
                return -1, 6
            if 101 <= a < 102:
                return 0, 5
            if 102 <= a < 4294967295:
                return -1, 6
        if q == 8:
            if 0 <= a < 43:
                return -1, 6
            if 43 <= a < 44:
                return -1, 6
            if 44 <= a < 45:
                return -1, 6
            if 45 <= a < 46:
                return -1, 6
            if 46 <= a < 47:
                return 0, 7
            if 47 <= a < 48:
                return -1, 6
            if 48 <= a < 49:
                return 1, 8
            if 49 <= a < 58:
                return 1, 8
            if 58 <= a < 69:
                return -1, 6
            if 69 <= a < 70:
                return 0, 5
            if 70 <= a < 101:
                return -1, 6
            if 101 <= a < 102:
                return 0, 5
            if 102 <= a < 4294967295:
                return -1, 6
        if q == 1:
            if 0 <= a < 43:
                return -1, 6
            if 43 <= a < 44:
                return -1, 6
            if 44 <= a < 45:
                return -1, 6
            if 45 <= a < 46:
                return -1, 6
            if 46 <= a < 47:
                return -1, 6
            if 47 <= a < 48:
                return -1, 6
            if 48 <= a < 49:
                return -1, 6
            if 49 <= a < 58:
                return -1, 6
            if 58 <= a < 69:
                return -1, 6
            if 69 <= a < 70:
                return -1, 6
            if 70 <= a < 101:
                return -1, 6
            if 101 <= a < 102:
                return -1, 6
            if 102 <= a < 4294967295:
                return -1, 6
        if q == 2:
            if 0 <= a < 43:
                return -1, 6
            if 43 <= a < 44:
                return -1, 6
            if 44 <= a < 45:
                return -1, 6
            if 45 <= a < 46:
                return -1, 6
            if 46 <= a < 47:
                return -1, 6
            if 47 <= a < 48:
                return -1, 6
            if 48 <= a < 49:
                return 1, 1
            if 49 <= a < 58:
                return 1, 4
            if 58 <= a < 69:
                return -1, 6
            if 69 <= a < 70:
                return -1, 6
            if 70 <= a < 101:
                return -1, 6
            if 101 <= a < 102:
                return -1, 6
            if 102 <= a < 4294967295:
                return -1, 6
        if q == 4:
            if 0 <= a < 43:
                return -1, 6
            if 43 <= a < 44:
                return -1, 6
            if 44 <= a < 45:
                return -1, 6
            if 45 <= a < 46:
                return -1, 6
            if 46 <= a < 47:
                return -1, 6
            if 47 <= a < 48:
                return -1, 6
            if 48 <= a < 49:
                return 1, 4
            if 49 <= a < 58:
                return 1, 4
            if 58 <= a < 69:
                return -1, 6
            if 69 <= a < 70:
                return -1, 6
            if 70 <= a < 101:
                return -1, 6
            if 101 <= a < 102:
                return -1, 6
            if 102 <= a < 4294967295:
                return -1, 6
        if q == 7:
            if 0 <= a < 43:
                return -1, 6
            if 43 <= a < 44:
                return -1, 6
            if 44 <= a < 45:
                return -1, 6
            if 45 <= a < 46:
                return -1, 6
            if 46 <= a < 47:
                return -1, 6
            if 47 <= a < 48:
                return -1, 6
            if 48 <= a < 49:
                return 1, 9
            if 49 <= a < 58:
                return 1, 9
            if 58 <= a < 69:
                return -1, 6
            if 69 <= a < 70:
                return -1, 6
            if 70 <= a < 101:
                return -1, 6
            if 101 <= a < 102:
                return -1, 6
            if 102 <= a < 4294967295:
                return -1, 6
        if q == 5:
            if 0 <= a < 43:
                return -1, 6
            if 43 <= a < 44:
                return 0, 2
            if 44 <= a < 45:
                return -1, 6
            if 45 <= a < 46:
                return 0, 2
            if 46 <= a < 47:
                return -1, 6
            if 47 <= a < 48:
                return -1, 6
            if 48 <= a < 49:
                return 1, 1
            if 49 <= a < 58:
                return 1, 4
            if 58 <= a < 69:
                return -1, 6
            if 69 <= a < 70:
                return -1, 6
            if 70 <= a < 101:
                return -1, 6
            if 101 <= a < 102:
                return -1, 6
            if 102 <= a < 4294967295:
                return -1, 6
        if q == 9:
            if 0 <= a < 43:
                return -1, 6
            if 43 <= a < 44:
                return -1, 6
            if 44 <= a < 45:
                return -1, 6
            if 45 <= a < 46:
                return -1, 6
            if 46 <= a < 47:
                return -1, 6
            if 47 <= a < 48:
                return -1, 6
            if 48 <= a < 49:
                return 1, 9
            if 49 <= a < 58:
                return 1, 9
            if 58 <= a < 69:
                return -1, 6
            if 69 <= a < 70:
                return 0, 5
            if 70 <= a < 101:
                return -1, 6
            if 101 <= a < 102:
                return 0, 5
            if 102 <= a < 4294967295:
                return -1, 6
        return -1, 2

class Character(BaseRecognizer):
    def __init__(self, io=InputStack()):
        super().__init__(Token.CHARACTER, False, 5, io)

    def next(self, q, a):
        if q == 0:
            if 0 <= a < 32:
                return -1, 5
            if 32 <= a < 39:
                return -1, 5
            if 39 <= a < 40:
                return 0, 3
            if 40 <= a < 92:
                return -1, 5
            if 92 <= a < 93:
                return -1, 5
            if 93 <= a < 110:
                return -1, 5
            if 110 <= a < 111:
                return -1, 5
            if 111 <= a < 116:
                return -1, 5
            if 116 <= a < 117:
                return -1, 5
            if 117 <= a < 127:
                return -1, 5
            if 127 <= a < 4294967295:
                return -1, 5
        if q == 3:
            if 0 <= a < 32:
                return -1, 5
            if 32 <= a < 39:
                return 0, 2
            if 39 <= a < 40:
                return -1, 5
            if 40 <= a < 92:
                return 0, 2
            if 92 <= a < 93:
                return 0, 4
            if 93 <= a < 110:
                return 0, 2
            if 110 <= a < 111:
                return 0, 2
            if 111 <= a < 116:
                return 0, 2
            if 116 <= a < 117:
                return 0, 2
            if 117 <= a < 127:
                return 0, 2
            if 127 <= a < 4294967295:
                return -1, 5
        if q == 1:
            if 0 <= a < 32:
                return -1, 5
            if 32 <= a < 39:
                return -1, 5
            if 39 <= a < 40:
                return -1, 5
            if 40 <= a < 92:
                return -1, 5
            if 92 <= a < 93:
                return -1, 5
            if 93 <= a < 110:
                return -1, 5
            if 110 <= a < 111:
                return -1, 5
            if 111 <= a < 116:
                return -1, 5
            if 116 <= a < 117:
                return -1, 5
            if 117 <= a < 127:
                return -1, 5
            if 127 <= a < 4294967295:
                return -1, 5
        if q == 2:
            if 0 <= a < 32:
                return -1, 5
            if 32 <= a < 39:
                return -1, 5
            if 39 <= a < 40:
                return 1, 1
            if 40 <= a < 92:
                return -1, 5
            if 92 <= a < 93:
                return -1, 5
            if 93 <= a < 110:
                return -1, 5
            if 110 <= a < 111:
                return -1, 5
            if 111 <= a < 116:
                return -1, 5
            if 116 <= a < 117:
                return -1, 5
            if 117 <= a < 127:
                return -1, 5
            if 127 <= a < 4294967295:
                return -1, 5
        if q == 4:
            if 0 <= a < 32:
                return -1, 5
            if 32 <= a < 39:
                return -1, 5
            if 39 <= a < 40:
                return 0, 2
            if 40 <= a < 92:
                return -1, 5
            if 92 <= a < 93:
                return 0, 2
            if 93 <= a < 110:
                return -1, 5
            if 110 <= a < 111:
                return 0, 2
            if 111 <= a < 116:
                return -1, 5
            if 116 <= a < 117:
                return 0, 2
            if 117 <= a < 127:
                return -1, 5
            if 127 <= a < 4294967295:
                return -1, 5
        return -1, 2

class String(BaseRecognizer):
    def __init__(self, io=InputStack()):
        super().__init__(Token.STRING, False, 4, io)

    def next(self, q, a):
        if q == 0:
            if 0 <= a < 32:
                return -1, 4
            if 32 <= a < 34:
                return -1, 4
            if 34 <= a < 35:
                return 0, 2
            if 35 <= a < 92:
                return -1, 4
            if 92 <= a < 93:
                return -1, 4
            if 93 <= a < 127:
                return -1, 4
            if 127 <= a < 4294967295:
                return -1, 4
        if q == 2:
            if 0 <= a < 32:
                return -1, 4
            if 32 <= a < 34:
                return 0, 2
            if 34 <= a < 35:
                return 1, 1
            if 35 <= a < 92:
                return 0, 2
            if 92 <= a < 93:
                return 0, 3
            if 93 <= a < 127:
                return 0, 2
            if 127 <= a < 4294967295:
                return -1, 4
        if q == 1:
            if 0 <= a < 32:
                return -1, 4
            if 32 <= a < 34:
                return -1, 4
            if 34 <= a < 35:
                return -1, 4
            if 35 <= a < 92:
                return -1, 4
            if 92 <= a < 93:
                return -1, 4
            if 93 <= a < 127:
                return -1, 4
            if 127 <= a < 4294967295:
                return -1, 4
        if q == 3:
            if 0 <= a < 32:
                return -1, 4
            if 32 <= a < 34:
                return 0, 2
            if 34 <= a < 35:
                return 0, 2
            if 35 <= a < 92:
                return 0, 2
            if 92 <= a < 93:
                return 0, 2
            if 93 <= a < 127:
                return 0, 2
            if 127 <= a < 4294967295:
                return -1, 4
        return -1, 2

class Dot(BaseRecognizer):
    def __init__(self, io=InputStack()):
        super().__init__(Token.DOT, False, 2, io)

    def next(self, q, a):
        if q == 0:
            if 0 <= a < 46:
                return -1, 2
            if 46 <= a < 47:
                return 1, 1
            if 47 <= a < 4294967295:
                return -1, 2
        if q == 1:
            if 0 <= a < 46:
                return -1, 2
            if 46 <= a < 47:
                return -1, 2
            if 47 <= a < 4294967295:
                return -1, 2
        return -1, 2

class Lparen(BaseRecognizer):
    def __init__(self, io=InputStack()):
        super().__init__(Token.LPAREN, False, 2, io)

    def next(self, q, a):
        if q == 0:
            if 0 <= a < 40:
                return -1, 2
            if 40 <= a < 41:
                return 1, 1
            if 41 <= a < 4294967295:
                return -1, 2
        if q == 1:
            if 0 <= a < 40:
                return -1, 2
            if 40 <= a < 41:
                return -1, 2
            if 41 <= a < 4294967295:
                return -1, 2
        return -1, 2

class Rparen(BaseRecognizer):
    def __init__(self, io=InputStack()):
        super().__init__(Token.RPAREN, False, 2, io)

    def next(self, q, a):
        if q == 0:
            if 0 <= a < 41:
                return -1, 2
            if 41 <= a < 42:
                return 1, 1
            if 42 <= a < 4294967295:
                return -1, 2
        if q == 1:
            if 0 <= a < 41:
                return -1, 2
            if 41 <= a < 42:
                return -1, 2
            if 42 <= a < 4294967295:
                return -1, 2
        return -1, 2

class Comma(BaseRecognizer):
    def __init__(self, io=InputStack()):
        super().__init__(Token.COMMA, False, 2, io)

    def next(self, q, a):
        if q == 0:
            if 0 <= a < 44:
                return -1, 2
            if 44 <= a < 45:
                return 1, 1
            if 45 <= a < 4294967295:
                return -1, 2
        if q == 1:
            if 0 <= a < 44:
                return -1, 2
            if 44 <= a < 45:
                return -1, 2
            if 45 <= a < 4294967295:
                return -1, 2
        return -1, 2

class Star(BaseRecognizer):
    def __init__(self, io=InputStack()):
        super().__init__(Token.STAR, False, 2, io)

    def next(self, q, a):
        if q == 0:
            if 0 <= a < 42:
                return -1, 2
            if 42 <= a < 43:
                return 1, 1
            if 43 <= a < 4294967295:
                return -1, 2
        if q == 1:
            if 0 <= a < 42:
                return -1, 2
            if 42 <= a < 43:
                return -1, 2
            if 43 <= a < 4294967295:
                return -1, 2
        return -1, 2

class Equal(BaseRecognizer):
    def __init__(self, io=InputStack()):
        super().__init__(Token.EQUAL, False, 2, io)

    def next(self, q, a):
        if q == 0:
            if 0 <= a < 61:
                return -1, 2
            if 61 <= a < 62:
                return 1, 1
            if 62 <= a < 4294967295:
                return -1, 2
        if q == 1:
            if 0 <= a < 61:
                return -1, 2
            if 61 <= a < 62:
                return -1, 2
            if 62 <= a < 4294967295:
                return -1, 2
        return -1, 2

class Lbrace(BaseRecognizer):
    def __init__(self, io=InputStack()):
        super().__init__(Token.LBRACE, False, 2, io)

    def next(self, q, a):
        if q == 0:
            if 0 <= a < 123:
                return -1, 2
            if 123 <= a < 124:
                return 1, 1
            if 124 <= a < 4294967295:
                return -1, 2
        if q == 1:
            if 0 <= a < 123:
                return -1, 2
            if 123 <= a < 124:
                return -1, 2
            if 124 <= a < 4294967295:
                return -1, 2
        return -1, 2

class Rbrace(BaseRecognizer):
    def __init__(self, io=InputStack()):
        super().__init__(Token.RBRACE, False, 2, io)

    def next(self, q, a):
        if q == 0:
            if 0 <= a < 125:
                return -1, 2
            if 125 <= a < 126:
                return 1, 1
            if 126 <= a < 4294967295:
                return -1, 2
        if q == 1:
            if 0 <= a < 125:
                return -1, 2
            if 125 <= a < 126:
                return -1, 2
            if 126 <= a < 4294967295:
                return -1, 2
        return -1, 2

class Colon(BaseRecognizer):
    def __init__(self, io=InputStack()):
        super().__init__(Token.COLON, False, 2, io)

    def next(self, q, a):
        if q == 0:
            if 0 <= a < 58:
                return -1, 2
            if 58 <= a < 59:
                return 1, 1
            if 59 <= a < 4294967295:
                return -1, 2
        if q == 1:
            if 0 <= a < 58:
                return -1, 2
            if 58 <= a < 59:
                return -1, 2
            if 59 <= a < 4294967295:
                return -1, 2
        return -1, 2

class Lbracket(BaseRecognizer):
    def __init__(self, io=InputStack()):
        super().__init__(Token.LBRACKET, False, 2, io)

    def next(self, q, a):
        if q == 0:
            if 0 <= a < 91:
                return -1, 2
            if 91 <= a < 92:
                return 1, 1
            if 92 <= a < 4294967295:
                return -1, 2
        if q == 1:
            if 0 <= a < 91:
                return -1, 2
            if 91 <= a < 92:
                return -1, 2
            if 92 <= a < 4294967295:
                return -1, 2
        return -1, 2

class Rbracket(BaseRecognizer):
    def __init__(self, io=InputStack()):
        super().__init__(Token.RBRACKET, False, 2, io)

    def next(self, q, a):
        if q == 0:
            if 0 <= a < 93:
                return -1, 2
            if 93 <= a < 94:
                return 1, 1
            if 94 <= a < 4294967295:
                return -1, 2
        if q == 1:
            if 0 <= a < 93:
                return -1, 2
            if 93 <= a < 94:
                return -1, 2
            if 94 <= a < 4294967295:
                return -1, 2
        return -1, 2

class Plus(BaseRecognizer):
    def __init__(self, io=InputStack()):
        super().__init__(Token.PLUS, False, 2, io)

    def next(self, q, a):
        if q == 0:
            if 0 <= a < 43:
                return -1, 2
            if 43 <= a < 44:
                return 1, 1
            if 44 <= a < 4294967295:
                return -1, 2
        if q == 1:
            if 0 <= a < 43:
                return -1, 2
            if 43 <= a < 44:
                return -1, 2
            if 44 <= a < 4294967295:
                return -1, 2
        return -1, 2

class Minus(BaseRecognizer):
    def __init__(self, io=InputStack()):
        super().__init__(Token.MINUS, False, 2, io)

    def next(self, q, a):
        if q == 0:
            if 0 <= a < 45:
                return -1, 2
            if 45 <= a < 46:
                return 1, 1
            if 46 <= a < 4294967295:
                return -1, 2
        if q == 1:
            if 0 <= a < 45:
                return -1, 2
            if 45 <= a < 46:
                return -1, 2
            if 46 <= a < 4294967295:
                return -1, 2
        return -1, 2

class Slash(BaseRecognizer):
    def __init__(self, io=InputStack()):
        super().__init__(Token.SLASH, False, 2, io)

    def next(self, q, a):
        if q == 0:
            if 0 <= a < 47:
                return -1, 2
            if 47 <= a < 48:
                return 1, 1
            if 48 <= a < 4294967295:
                return -1, 2
        if q == 1:
            if 0 <= a < 47:
                return -1, 2
            if 47 <= a < 48:
                return -1, 2
            if 48 <= a < 4294967295:
                return -1, 2
        return -1, 2

class Lessthan(BaseRecognizer):
    def __init__(self, io=InputStack()):
        super().__init__(Token.LESSTHAN, False, 2, io)

    def next(self, q, a):
        if q == 0:
            if 0 <= a < 60:
                return -1, 2
            if 60 <= a < 61:
                return 1, 1
            if 61 <= a < 4294967295:
                return -1, 2
        if q == 1:
            if 0 <= a < 60:
                return -1, 2
            if 60 <= a < 61:
                return -1, 2
            if 61 <= a < 4294967295:
                return -1, 2
        return -1, 2

class Greaterthan(BaseRecognizer):
    def __init__(self, io=InputStack()):
        super().__init__(Token.GREATERTHAN, False, 2, io)

    def next(self, q, a):
        if q == 0:
            if 0 <= a < 62:
                return -1, 2
            if 62 <= a < 63:
                return 1, 1
            if 63 <= a < 4294967295:
                return -1, 2
        if q == 1:
            if 0 <= a < 62:
                return -1, 2
            if 62 <= a < 63:
                return -1, 2
            if 63 <= a < 4294967295:
                return -1, 2
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
