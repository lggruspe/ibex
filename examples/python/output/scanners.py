class TransitionError(Exception):
    pass

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
        """Transition and set checkpoint if visiting an accept state."""
        if checkpoint:
            self.checkpoint = []
            self.accepts = True
        self.checkpoint.append(next_state)

    def next(self, char):
        """State transition on input char, returns True if successful."""
        if self.state != -1:
            try:
                getattr(self, f"s{self.state}")(ord(char))
                return self.state != -1
            except:
                self.change_state(-1)
                return False
        return False

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
            self.change_state(1, checkpoint=True)
        elif char == 95:
            self.change_state(1, checkpoint=True)
        elif 97 <= char <= 122:
            self.change_state(1, checkpoint=True)
        else:
            raise TransitionError

    def s1(self, char):
        if 48 <= char <= 57:
            self.change_state(1, checkpoint=True)
        elif 65 <= char <= 90:
            self.change_state(1, checkpoint=True)
        elif char == 95:
            self.change_state(1, checkpoint=True)
        elif 97 <= char <= 122:
            self.change_state(1, checkpoint=True)
        else:
            raise TransitionError

class WhitespaceScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "whitespace"

    def s0(self, char):
        if char == 9:
            self.change_state(1, checkpoint=True)
        elif char == 10:
            self.change_state(1, checkpoint=True)
        elif char == 32:
            self.change_state(1, checkpoint=True)
        else:
            raise TransitionError

    def s1(self, char):
        raise TransitionError

class NumberScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "number"

    def s0(self, char):
        if char == 48:
            self.change_state(5, checkpoint=True)
        elif 49 <= char <= 57:
            self.change_state(1, checkpoint=True)
        else:
            raise TransitionError

    def s5(self, char):
        if char == 46:
            self.change_state(7)
        elif char == 69:
            self.change_state(2)
        elif char == 101:
            self.change_state(2)
        else:
            raise TransitionError

    def s1(self, char):
        if char == 46:
            self.change_state(7)
        elif char == 48:
            self.change_state(1, checkpoint=True)
        elif 49 <= char <= 57:
            self.change_state(1, checkpoint=True)
        elif char == 69:
            self.change_state(2)
        elif char == 101:
            self.change_state(2)
        else:
            raise TransitionError

    def s7(self, char):
        if char == 48:
            self.change_state(3, checkpoint=True)
        elif 49 <= char <= 57:
            self.change_state(3, checkpoint=True)
        else:
            raise TransitionError

    def s2(self, char):
        if char == 43:
            self.change_state(4)
        elif char == 45:
            self.change_state(4)
        elif char == 48:
            self.change_state(6, checkpoint=True)
        elif 49 <= char <= 57:
            self.change_state(8, checkpoint=True)
        else:
            raise TransitionError

    def s4(self, char):
        if char == 48:
            self.change_state(6, checkpoint=True)
        elif 49 <= char <= 57:
            self.change_state(8, checkpoint=True)
        else:
            raise TransitionError

    def s6(self, char):
        raise TransitionError

    def s8(self, char):
        if char == 48:
            self.change_state(8, checkpoint=True)
        elif 49 <= char <= 57:
            self.change_state(8, checkpoint=True)
        else:
            raise TransitionError

    def s3(self, char):
        if char == 48:
            self.change_state(3, checkpoint=True)
        elif 49 <= char <= 57:
            self.change_state(3, checkpoint=True)
        elif char == 69:
            self.change_state(2)
        elif char == 101:
            self.change_state(2)
        else:
            raise TransitionError

class CharacterScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "character"

    def s0(self, char):
        if char == 39:
            self.change_state(2)
        else:
            raise TransitionError

    def s2(self, char):
        if 32 <= char <= 38:
            self.change_state(3)
        elif 40 <= char <= 91:
            self.change_state(3)
        elif char == 92:
            self.change_state(4)
        elif 93 <= char <= 126:
            self.change_state(3)
        else:
            raise TransitionError

    def s1(self, char):
        raise TransitionError

    def s3(self, char):
        if char == 39:
            self.change_state(1, checkpoint=True)
        else:
            raise TransitionError

    def s4(self, char):
        if char == 39:
            self.change_state(3)
        elif char == 92:
            self.change_state(3)
        elif 93 <= char <= 126:
            self.change_state(3)
        else:
            raise TransitionError

class StringScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "string"

    def s0(self, char):
        if char == 34:
            self.change_state(2)
        else:
            raise TransitionError

    def s2(self, char):
        if 32 <= char <= 33:
            self.change_state(2)
        elif char == 34:
            self.change_state(1, checkpoint=True)
        elif 35 <= char <= 91:
            self.change_state(2)
        elif char == 92:
            self.change_state(3)
        elif 93 <= char <= 126:
            self.change_state(2)
        else:
            raise TransitionError

    def s1(self, char):
        raise TransitionError

    def s3(self, char):
        if 32 <= char <= 33:
            self.change_state(2)
        elif char == 34:
            self.change_state(2)
        elif 35 <= char <= 91:
            self.change_state(2)
        elif char == 92:
            self.change_state(2)
        elif 93 <= char <= 126:
            self.change_state(2)
        else:
            raise TransitionError

class DotScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "dot"

    def s0(self, char):
        if char == 46:
            self.change_state(1, checkpoint=True)
        else:
            raise TransitionError

    def s1(self, char):
        raise TransitionError

class LparenScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "lparen"

    def s0(self, char):
        if char == 40:
            self.change_state(1, checkpoint=True)
        else:
            raise TransitionError

    def s1(self, char):
        raise TransitionError

class RparenScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "rparen"

    def s0(self, char):
        if char == 41:
            self.change_state(1, checkpoint=True)
        else:
            raise TransitionError

    def s1(self, char):
        raise TransitionError

class CommaScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "comma"

    def s0(self, char):
        if char == 44:
            self.change_state(1, checkpoint=True)
        else:
            raise TransitionError

    def s1(self, char):
        raise TransitionError

class StarScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "star"

    def s0(self, char):
        if char == 42:
            self.change_state(1, checkpoint=True)
        else:
            raise TransitionError

    def s1(self, char):
        raise TransitionError

class EqualScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "equal"

    def s0(self, char):
        if char == 61:
            self.change_state(1, checkpoint=True)
        else:
            raise TransitionError

    def s1(self, char):
        raise TransitionError

class LbraceScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "lbrace"

    def s0(self, char):
        if char == 123:
            self.change_state(1, checkpoint=True)
        else:
            raise TransitionError

    def s1(self, char):
        raise TransitionError

class RbraceScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "rbrace"

    def s0(self, char):
        if char == 125:
            self.change_state(1, checkpoint=True)
        else:
            raise TransitionError

    def s1(self, char):
        raise TransitionError

class ColonScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "colon"

    def s0(self, char):
        if char == 58:
            self.change_state(1, checkpoint=True)
        else:
            raise TransitionError

    def s1(self, char):
        raise TransitionError

class LbracketScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "lbracket"

    def s0(self, char):
        if char == 91:
            self.change_state(1, checkpoint=True)
        else:
            raise TransitionError

    def s1(self, char):
        raise TransitionError

class RbracketScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "rbracket"

    def s0(self, char):
        if char == 93:
            self.change_state(1, checkpoint=True)
        else:
            raise TransitionError

    def s1(self, char):
        raise TransitionError

class PlusScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "plus"

    def s0(self, char):
        if char == 43:
            self.change_state(1, checkpoint=True)
        else:
            raise TransitionError

    def s1(self, char):
        raise TransitionError

class MinusScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "minus"

    def s0(self, char):
        if char == 45:
            self.change_state(1, checkpoint=True)
        else:
            raise TransitionError

    def s1(self, char):
        raise TransitionError

class SlashScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "slash"

    def s0(self, char):
        if char == 47:
            self.change_state(1, checkpoint=True)
        else:
            raise TransitionError

    def s1(self, char):
        raise TransitionError

class LessthanScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "lessthan"

    def s0(self, char):
        if char == 60:
            self.change_state(1, checkpoint=True)
        else:
            raise TransitionError

    def s1(self, char):
        raise TransitionError

class GreaterthanScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "greaterthan"

    def s0(self, char):
        if char == 62:
            self.change_state(1, checkpoint=True)
        else:
            raise TransitionError

    def s1(self, char):
        raise TransitionError

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
