class Scanner:
    """Base class for scangen-generated scanners."""

    def __init__(self):
        """
        checkpoint: every state visited since the last accept state
        accepts: True if an accept state is ever visited
        """
        self.checkpoint = [0]
        self.token = ""
        self.accepts = False

    @property
    def state(self):
        return self.checkpoint[-1] if self.checkpoint else -1

    def change_state(self, next_state, checkpoint=False):
        """Transition and set checkpoint if visiting an accept state.

        Return True if next_state is not an error state.
        """
        if checkpoint:
            self.checkpoint = []
            self.accepts = True
        self.checkpoint.append(next_state)
        return next_state != -1

    def next(self, char: int):
        """State transition on input char, returns True if successful."""
        try:
            getattr(self, f"s{self.state}")(char)
            return self.state != -1
        except:
            return self.change_state(-1)

    def backtrack_steps(self):
        """Number of steps from most recent accept or initial state.."""
        return len(self.checkpoint) - 1

    def __repr__(self):
        return f"<Scanner {self.token} state:{self.state} checkpoint:{self.checkpoint}>"

class IdentifierScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "identifier"

    def s0(self, char):
        if 65 <= char <= 90:
            return self.change_state(1, checkpoint=True)
        if char == 95:
            return self.change_state(1, checkpoint=True)
        if 97 <= char <= 122:
            return self.change_state(1, checkpoint=True)
        return self.change_state(-1)

    def s1(self, char):
        if 48 <= char <= 57:
            return self.change_state(1, checkpoint=True)
        if 65 <= char <= 90:
            return self.change_state(1, checkpoint=True)
        if char == 95:
            return self.change_state(1, checkpoint=True)
        if 97 <= char <= 122:
            return self.change_state(1, checkpoint=True)
        return self.change_state(-1)

class WhitespaceScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "whitespace"

    def s0(self, char):
        if char == 9:
            return self.change_state(1, checkpoint=True)
        if char == 10:
            return self.change_state(1, checkpoint=True)
        if char == 32:
            return self.change_state(1, checkpoint=True)
        return self.change_state(-1)

    def s1(self, char):
        return self.change_state(-1)

class NumberScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "number"

    def s0(self, char):
        if char == 48:
            return self.change_state(5, checkpoint=True)
        if 49 <= char <= 57:
            return self.change_state(1, checkpoint=True)
        return self.change_state(-1)

    def s5(self, char):
        if char == 46:
            return self.change_state(7)
        if char == 69:
            return self.change_state(2)
        if char == 101:
            return self.change_state(2)
        return self.change_state(-1)

    def s1(self, char):
        if char == 46:
            return self.change_state(7)
        if char == 48:
            return self.change_state(1, checkpoint=True)
        if 49 <= char <= 57:
            return self.change_state(1, checkpoint=True)
        if char == 69:
            return self.change_state(2)
        if char == 101:
            return self.change_state(2)
        return self.change_state(-1)

    def s7(self, char):
        if char == 48:
            return self.change_state(3, checkpoint=True)
        if 49 <= char <= 57:
            return self.change_state(3, checkpoint=True)
        return self.change_state(-1)

    def s2(self, char):
        if char == 43:
            return self.change_state(4)
        if char == 45:
            return self.change_state(4)
        if char == 48:
            return self.change_state(6, checkpoint=True)
        if 49 <= char <= 57:
            return self.change_state(8, checkpoint=True)
        return self.change_state(-1)

    def s4(self, char):
        if char == 48:
            return self.change_state(6, checkpoint=True)
        if 49 <= char <= 57:
            return self.change_state(8, checkpoint=True)
        return self.change_state(-1)

    def s6(self, char):
        return self.change_state(-1)

    def s8(self, char):
        if char == 48:
            return self.change_state(8, checkpoint=True)
        if 49 <= char <= 57:
            return self.change_state(8, checkpoint=True)
        return self.change_state(-1)

    def s3(self, char):
        if char == 48:
            return self.change_state(3, checkpoint=True)
        if 49 <= char <= 57:
            return self.change_state(3, checkpoint=True)
        if char == 69:
            return self.change_state(2)
        if char == 101:
            return self.change_state(2)
        return self.change_state(-1)

class CharacterScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "character"

    def s0(self, char):
        if char == 39:
            return self.change_state(2)
        return self.change_state(-1)

    def s2(self, char):
        if 32 <= char <= 38:
            return self.change_state(3)
        if 40 <= char <= 91:
            return self.change_state(3)
        if char == 92:
            return self.change_state(4)
        if 93 <= char <= 126:
            return self.change_state(3)
        return self.change_state(-1)

    def s1(self, char):
        return self.change_state(-1)

    def s3(self, char):
        if char == 39:
            return self.change_state(1, checkpoint=True)
        return self.change_state(-1)

    def s4(self, char):
        if char == 39:
            return self.change_state(3)
        if char == 92:
            return self.change_state(3)
        if 93 <= char <= 126:
            return self.change_state(3)
        return self.change_state(-1)

class StringScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "string"

    def s0(self, char):
        if char == 34:
            return self.change_state(2)
        return self.change_state(-1)

    def s2(self, char):
        if 32 <= char <= 33:
            return self.change_state(2)
        if char == 34:
            return self.change_state(1, checkpoint=True)
        if 35 <= char <= 91:
            return self.change_state(2)
        if char == 92:
            return self.change_state(3)
        if 93 <= char <= 126:
            return self.change_state(2)
        return self.change_state(-1)

    def s1(self, char):
        return self.change_state(-1)

    def s3(self, char):
        if 32 <= char <= 33:
            return self.change_state(2)
        if char == 34:
            return self.change_state(2)
        if 35 <= char <= 91:
            return self.change_state(2)
        if char == 92:
            return self.change_state(2)
        if 93 <= char <= 126:
            return self.change_state(2)
        return self.change_state(-1)

class DotScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "dot"

    def s0(self, char):
        if char == 46:
            return self.change_state(1, checkpoint=True)
        return self.change_state(-1)

    def s1(self, char):
        return self.change_state(-1)

class LparenScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "lparen"

    def s0(self, char):
        if char == 40:
            return self.change_state(1, checkpoint=True)
        return self.change_state(-1)

    def s1(self, char):
        return self.change_state(-1)

class RparenScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "rparen"

    def s0(self, char):
        if char == 41:
            return self.change_state(1, checkpoint=True)
        return self.change_state(-1)

    def s1(self, char):
        return self.change_state(-1)

class CommaScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "comma"

    def s0(self, char):
        if char == 44:
            return self.change_state(1, checkpoint=True)
        return self.change_state(-1)

    def s1(self, char):
        return self.change_state(-1)

class StarScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "star"

    def s0(self, char):
        if char == 42:
            return self.change_state(1, checkpoint=True)
        return self.change_state(-1)

    def s1(self, char):
        return self.change_state(-1)

class EqualScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "equal"

    def s0(self, char):
        if char == 61:
            return self.change_state(1, checkpoint=True)
        return self.change_state(-1)

    def s1(self, char):
        return self.change_state(-1)

class LbraceScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "lbrace"

    def s0(self, char):
        if char == 123:
            return self.change_state(1, checkpoint=True)
        return self.change_state(-1)

    def s1(self, char):
        return self.change_state(-1)

class RbraceScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "rbrace"

    def s0(self, char):
        if char == 125:
            return self.change_state(1, checkpoint=True)
        return self.change_state(-1)

    def s1(self, char):
        return self.change_state(-1)

class ColonScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "colon"

    def s0(self, char):
        if char == 58:
            return self.change_state(1, checkpoint=True)
        return self.change_state(-1)

    def s1(self, char):
        return self.change_state(-1)

class LbracketScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "lbracket"

    def s0(self, char):
        if char == 91:
            return self.change_state(1, checkpoint=True)
        return self.change_state(-1)

    def s1(self, char):
        return self.change_state(-1)

class RbracketScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "rbracket"

    def s0(self, char):
        if char == 93:
            return self.change_state(1, checkpoint=True)
        return self.change_state(-1)

    def s1(self, char):
        return self.change_state(-1)

class PlusScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "plus"

    def s0(self, char):
        if char == 43:
            return self.change_state(1, checkpoint=True)
        return self.change_state(-1)

    def s1(self, char):
        return self.change_state(-1)

class MinusScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "minus"

    def s0(self, char):
        if char == 45:
            return self.change_state(1, checkpoint=True)
        return self.change_state(-1)

    def s1(self, char):
        return self.change_state(-1)

class SlashScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "slash"

    def s0(self, char):
        if char == 47:
            return self.change_state(1, checkpoint=True)
        return self.change_state(-1)

    def s1(self, char):
        return self.change_state(-1)

class LessthanScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "lessthan"

    def s0(self, char):
        if char == 60:
            return self.change_state(1, checkpoint=True)
        return self.change_state(-1)

    def s1(self, char):
        return self.change_state(-1)

class GreaterthanScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "greaterthan"

    def s0(self, char):
        if char == 62:
            return self.change_state(1, checkpoint=True)
        return self.change_state(-1)

    def s1(self, char):
        return self.change_state(-1)

SCANNERS = {
    "identifier": IdentifierScanner,
    "whitespace": WhitespaceScanner,
    "number": NumberScanner,
    "character": CharacterScanner,
    "string": StringScanner,
    "dot": DotScanner,
    "lparen": LparenScanner,
    "rparen": RparenScanner,
    "comma": CommaScanner,
    "star": StarScanner,
    "equal": EqualScanner,
    "lbrace": LbraceScanner,
    "rbrace": RbraceScanner,
    "colon": ColonScanner,
    "lbracket": LbracketScanner,
    "rbracket": RbracketScanner,
    "plus": PlusScanner,
    "minus": MinusScanner,
    "slash": SlashScanner,
    "lessthan": LessthanScanner,
    "greaterthan": GreaterthanScanner,
}
