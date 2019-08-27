class TransitionError(Exception):
    pass

class Scanner:
    """Base class for scangen-generated scanners."""

    def __init__(self):
        self.checkpoint = [0]
        self.token = ""

    @property
    def state(self):
        return self.checkpoint[-1] if self.checkpoint else -1

    @state.setter
    def state(self, next_state):
        self.checkpoint.append(next_state)

    def next(self, char):
        """State transition on input char, returns True if successful."""
        if self.state != -1:
            try:
                getattr(self, f"s{self.state}")(ord(char))
                return self.state != -1
            except:
                self.state = -1
                return False
        return False

    def backtrack(self):
        """Backtrack to most recent accept state, return number of steps."""
        steps = 0
        while not self.accepts() and len(self.checkpoint) > 1:
            self.checkpoint.pop()
            steps += 1
        return steps

    def __repr__(self):
        return f"<Scanner {self.token} state:{self.state} checkpoint:{self.checkpoint}>"

    def accepts(self):
        raise NotImplementedError

class IdentifierScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "identifier"

    def accepts(self):
        return self.state in [1]

    def s0(self, char):
        if 65 <= char <= 90:
            self.state = 1
        elif char == 95:
            self.state = 1
        elif 97 <= char <= 122:
            self.state = 1
        else:
            raise TransitionError

    def s1(self, char):
        if 48 <= char <= 57:
            self.state = 1
        elif 65 <= char <= 90:
            self.state = 1
        elif char == 95:
            self.state = 1
        elif 97 <= char <= 122:
            self.state = 1
        else:
            raise TransitionError

class WhitespaceScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "whitespace"

    def accepts(self):
        return self.state in [1]

    def s0(self, char):
        if char == 9:
            self.state = 1
        elif char == 10:
            self.state = 1
        elif char == 32:
            self.state = 1
        else:
            raise TransitionError

    def s1(self, char):
        raise TransitionError

class NumberScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "number"

    def accepts(self):
        return self.state in [1, 3, 5, 6, 8]

    def s0(self, char):
        if char == 48:
            self.state = 5
        elif 49 <= char <= 57:
            self.state = 1
        else:
            raise TransitionError

    def s5(self, char):
        if char == 46:
            self.state = 7
        elif char == 69:
            self.state = 2
        elif char == 101:
            self.state = 2
        else:
            raise TransitionError

    def s1(self, char):
        if char == 46:
            self.state = 7
        elif char == 48:
            self.state = 1
        elif 49 <= char <= 57:
            self.state = 1
        elif char == 69:
            self.state = 2
        elif char == 101:
            self.state = 2
        else:
            raise TransitionError

    def s7(self, char):
        if char == 48:
            self.state = 3
        elif 49 <= char <= 57:
            self.state = 3
        else:
            raise TransitionError

    def s2(self, char):
        if char == 43:
            self.state = 4
        elif char == 45:
            self.state = 4
        elif char == 48:
            self.state = 6
        elif 49 <= char <= 57:
            self.state = 8
        else:
            raise TransitionError

    def s4(self, char):
        if char == 48:
            self.state = 6
        elif 49 <= char <= 57:
            self.state = 8
        else:
            raise TransitionError

    def s6(self, char):
        raise TransitionError

    def s8(self, char):
        if char == 48:
            self.state = 8
        elif 49 <= char <= 57:
            self.state = 8
        else:
            raise TransitionError

    def s3(self, char):
        if char == 48:
            self.state = 3
        elif 49 <= char <= 57:
            self.state = 3
        elif char == 69:
            self.state = 2
        elif char == 101:
            self.state = 2
        else:
            raise TransitionError

class CharacterScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "character"

    def accepts(self):
        return self.state in [1]

    def s0(self, char):
        if char == 39:
            self.state = 2
        else:
            raise TransitionError

    def s2(self, char):
        if 32 <= char <= 38:
            self.state = 3
        elif 40 <= char <= 91:
            self.state = 3
        elif char == 92:
            self.state = 4
        elif 93 <= char <= 126:
            self.state = 3
        else:
            raise TransitionError

    def s1(self, char):
        raise TransitionError

    def s3(self, char):
        if char == 39:
            self.state = 1
        else:
            raise TransitionError

    def s4(self, char):
        if char == 39:
            self.state = 3
        elif char == 92:
            self.state = 3
        elif 93 <= char <= 126:
            self.state = 3
        else:
            raise TransitionError

class StringScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "string"

    def accepts(self):
        return self.state in [1]

    def s0(self, char):
        if char == 34:
            self.state = 2
        else:
            raise TransitionError

    def s2(self, char):
        if 32 <= char <= 33:
            self.state = 2
        elif char == 34:
            self.state = 1
        elif 35 <= char <= 91:
            self.state = 2
        elif char == 92:
            self.state = 3
        elif 93 <= char <= 126:
            self.state = 2
        else:
            raise TransitionError

    def s1(self, char):
        raise TransitionError

    def s3(self, char):
        if 32 <= char <= 33:
            self.state = 2
        elif char == 34:
            self.state = 2
        elif 35 <= char <= 91:
            self.state = 2
        elif char == 92:
            self.state = 2
        elif 93 <= char <= 126:
            self.state = 2
        else:
            raise TransitionError

class DotScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "dot"

    def accepts(self):
        return self.state in [1]

    def s0(self, char):
        if char == 46:
            self.state = 1
        else:
            raise TransitionError

    def s1(self, char):
        raise TransitionError

class LparenScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "lparen"

    def accepts(self):
        return self.state in [1]

    def s0(self, char):
        if char == 40:
            self.state = 1
        else:
            raise TransitionError

    def s1(self, char):
        raise TransitionError

class RparenScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "rparen"

    def accepts(self):
        return self.state in [1]

    def s0(self, char):
        if char == 41:
            self.state = 1
        else:
            raise TransitionError

    def s1(self, char):
        raise TransitionError

class CommaScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "comma"

    def accepts(self):
        return self.state in [1]

    def s0(self, char):
        if char == 44:
            self.state = 1
        else:
            raise TransitionError

    def s1(self, char):
        raise TransitionError

class StarScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "star"

    def accepts(self):
        return self.state in [1]

    def s0(self, char):
        if char == 42:
            self.state = 1
        else:
            raise TransitionError

    def s1(self, char):
        raise TransitionError

class EqualScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "equal"

    def accepts(self):
        return self.state in [1]

    def s0(self, char):
        if char == 61:
            self.state = 1
        else:
            raise TransitionError

    def s1(self, char):
        raise TransitionError

class LbraceScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "lbrace"

    def accepts(self):
        return self.state in [1]

    def s0(self, char):
        if char == 123:
            self.state = 1
        else:
            raise TransitionError

    def s1(self, char):
        raise TransitionError

class RbraceScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "rbrace"

    def accepts(self):
        return self.state in [1]

    def s0(self, char):
        if char == 125:
            self.state = 1
        else:
            raise TransitionError

    def s1(self, char):
        raise TransitionError

class ColonScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "colon"

    def accepts(self):
        return self.state in [1]

    def s0(self, char):
        if char == 58:
            self.state = 1
        else:
            raise TransitionError

    def s1(self, char):
        raise TransitionError

class LbracketScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "lbracket"

    def accepts(self):
        return self.state in [1]

    def s0(self, char):
        if char == 91:
            self.state = 1
        else:
            raise TransitionError

    def s1(self, char):
        raise TransitionError

class RbracketScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "rbracket"

    def accepts(self):
        return self.state in [1]

    def s0(self, char):
        if char == 93:
            self.state = 1
        else:
            raise TransitionError

    def s1(self, char):
        raise TransitionError

class PlusScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "plus"

    def accepts(self):
        return self.state in [1]

    def s0(self, char):
        if char == 43:
            self.state = 1
        else:
            raise TransitionError

    def s1(self, char):
        raise TransitionError

class MinusScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "minus"

    def accepts(self):
        return self.state in [1]

    def s0(self, char):
        if char == 45:
            self.state = 1
        else:
            raise TransitionError

    def s1(self, char):
        raise TransitionError

class SlashScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "slash"

    def accepts(self):
        return self.state in [1]

    def s0(self, char):
        if char == 47:
            self.state = 1
        else:
            raise TransitionError

    def s1(self, char):
        raise TransitionError

class LessthanScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "lessthan"

    def accepts(self):
        return self.state in [1]

    def s0(self, char):
        if char == 60:
            self.state = 1
        else:
            raise TransitionError

    def s1(self, char):
        raise TransitionError

class GreaterthanScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "greaterthan"

    def accepts(self):
        return self.state in [1]

    def s0(self, char):
        if char == 62:
            self.state = 1
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
