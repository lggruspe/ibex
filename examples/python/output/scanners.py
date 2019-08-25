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
                return getattr(self, f"s{self.state}")(ord(char))
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

class EmptyScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "empty"

    def accepts(self):
        return self.state in [0]

    def s0(self, char):
        raise TransitionError

class IdentifierScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "identifier"

    def accepts(self):
        return self.state in [1]

    def s0(self, char):
        if 65 <= char <= 90:
            self.state = 1
            return self.state
        if 95 <= char <= 95:
            self.state = 1
            return self.state
        if 97 <= char <= 122:
            self.state = 1
            return self.state
        raise TransitionError

    def s1(self, char):
        if 48 <= char <= 57:
            self.state = 1
            return self.state
        if 65 <= char <= 90:
            self.state = 1
            return self.state
        if 95 <= char <= 95:
            self.state = 1
            return self.state
        if 97 <= char <= 122:
            self.state = 1
            return self.state
        raise TransitionError

class WhitespaceScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "whitespace"

    def accepts(self):
        return self.state in [1]

    def s0(self, char):
        if 9 <= char <= 9:
            self.state = 1
            return self.state
        if 10 <= char <= 10:
            self.state = 1
            return self.state
        if 32 <= char <= 32:
            self.state = 1
            return self.state
        raise TransitionError

    def s1(self, char):
        raise TransitionError

class IntegerScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "integer"

    def accepts(self):
        return self.state in [1, 2]

    def s0(self, char):
        if 48 <= char <= 48:
            self.state = 1
            return self.state
        if 49 <= char <= 57:
            self.state = 2
            return self.state
        raise TransitionError

    def s1(self, char):
        raise TransitionError

    def s2(self, char):
        if 48 <= char <= 48:
            self.state = 2
            return self.state
        if 49 <= char <= 57:
            self.state = 2
            return self.state
        raise TransitionError

class NumberScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "number"

    def accepts(self):
        return self.state in [1, 3, 5, 6, 8]

    def s0(self, char):
        if 48 <= char <= 48:
            self.state = 5
            return self.state
        if 49 <= char <= 57:
            self.state = 1
            return self.state
        raise TransitionError

    def s5(self, char):
        if 46 <= char <= 46:
            self.state = 7
            return self.state
        if 69 <= char <= 69:
            self.state = 2
            return self.state
        if 101 <= char <= 101:
            self.state = 2
            return self.state
        raise TransitionError

    def s1(self, char):
        if 46 <= char <= 46:
            self.state = 7
            return self.state
        if 48 <= char <= 48:
            self.state = 1
            return self.state
        if 49 <= char <= 57:
            self.state = 1
            return self.state
        if 69 <= char <= 69:
            self.state = 2
            return self.state
        if 101 <= char <= 101:
            self.state = 2
            return self.state
        raise TransitionError

    def s7(self, char):
        if 48 <= char <= 48:
            self.state = 3
            return self.state
        if 49 <= char <= 57:
            self.state = 3
            return self.state
        raise TransitionError

    def s2(self, char):
        if 43 <= char <= 43:
            self.state = 4
            return self.state
        if 45 <= char <= 45:
            self.state = 4
            return self.state
        if 48 <= char <= 48:
            self.state = 6
            return self.state
        if 49 <= char <= 57:
            self.state = 8
            return self.state
        raise TransitionError

    def s4(self, char):
        if 48 <= char <= 48:
            self.state = 6
            return self.state
        if 49 <= char <= 57:
            self.state = 8
            return self.state
        raise TransitionError

    def s6(self, char):
        raise TransitionError

    def s8(self, char):
        if 48 <= char <= 48:
            self.state = 8
            return self.state
        if 49 <= char <= 57:
            self.state = 8
            return self.state
        raise TransitionError

    def s3(self, char):
        if 48 <= char <= 48:
            self.state = 3
            return self.state
        if 49 <= char <= 57:
            self.state = 3
            return self.state
        if 69 <= char <= 69:
            self.state = 2
            return self.state
        if 101 <= char <= 101:
            self.state = 2
            return self.state
        raise TransitionError

class CharacterScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "character"

    def accepts(self):
        return self.state in [1]

    def s0(self, char):
        if 39 <= char <= 39:
            self.state = 2
            return self.state
        raise TransitionError

    def s2(self, char):
        if 32 <= char <= 38:
            self.state = 3
            return self.state
        if 40 <= char <= 91:
            self.state = 3
            return self.state
        if 92 <= char <= 92:
            self.state = 4
            return self.state
        if 93 <= char <= 126:
            self.state = 3
            return self.state
        raise TransitionError

    def s1(self, char):
        raise TransitionError

    def s3(self, char):
        if 39 <= char <= 39:
            self.state = 1
            return self.state
        raise TransitionError

    def s4(self, char):
        if 39 <= char <= 39:
            self.state = 3
            return self.state
        if 92 <= char <= 92:
            self.state = 3
            return self.state
        if 93 <= char <= 126:
            self.state = 3
            return self.state
        raise TransitionError

class StringScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "string"

    def accepts(self):
        return self.state in [1]

    def s0(self, char):
        if 34 <= char <= 34:
            self.state = 2
            return self.state
        raise TransitionError

    def s2(self, char):
        if 32 <= char <= 33:
            self.state = 2
            return self.state
        if 34 <= char <= 34:
            self.state = 1
            return self.state
        if 35 <= char <= 91:
            self.state = 2
            return self.state
        if 92 <= char <= 92:
            self.state = 3
            return self.state
        if 93 <= char <= 126:
            self.state = 2
            return self.state
        raise TransitionError

    def s1(self, char):
        raise TransitionError

    def s3(self, char):
        if 32 <= char <= 33:
            self.state = 2
            return self.state
        if 34 <= char <= 34:
            self.state = 2
            return self.state
        if 35 <= char <= 91:
            self.state = 2
            return self.state
        if 92 <= char <= 92:
            self.state = 2
            return self.state
        if 93 <= char <= 126:
            self.state = 2
            return self.state
        raise TransitionError


