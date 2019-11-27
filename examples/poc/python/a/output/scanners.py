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
        if 0 <= char < 48:
            return self.change_state(2)
        if 48 <= char < 58:
            return self.change_state(2)
        if 58 <= char < 65:
            return self.change_state(2)
        if 65 <= char < 91:
            return self.change_state(1, checkpoint=True)
        if 91 <= char < 95:
            return self.change_state(2)
        if 95 <= char < 96:
            return self.change_state(1, checkpoint=True)
        if 96 <= char < 97:
            return self.change_state(2)
        if 97 <= char < 123:
            return self.change_state(1, checkpoint=True)
        if 123 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

    def s2(self, char):
        if 0 <= char < 48:
            return self.change_state(2)
        if 48 <= char < 58:
            return self.change_state(2)
        if 58 <= char < 65:
            return self.change_state(2)
        if 65 <= char < 91:
            return self.change_state(2)
        if 91 <= char < 95:
            return self.change_state(2)
        if 95 <= char < 96:
            return self.change_state(2)
        if 96 <= char < 97:
            return self.change_state(2)
        if 97 <= char < 123:
            return self.change_state(2)
        if 123 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

    def s1(self, char):
        if 0 <= char < 48:
            return self.change_state(2)
        if 48 <= char < 58:
            return self.change_state(1, checkpoint=True)
        if 58 <= char < 65:
            return self.change_state(2)
        if 65 <= char < 91:
            return self.change_state(1, checkpoint=True)
        if 91 <= char < 95:
            return self.change_state(2)
        if 95 <= char < 96:
            return self.change_state(1, checkpoint=True)
        if 96 <= char < 97:
            return self.change_state(2)
        if 97 <= char < 123:
            return self.change_state(1, checkpoint=True)
        if 123 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

class WhitespaceScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "whitespace"

    def s0(self, char):
        if 0 <= char < 9:
            return self.change_state(2)
        if 9 <= char < 10:
            return self.change_state(1, checkpoint=True)
        if 10 <= char < 11:
            return self.change_state(1, checkpoint=True)
        if 11 <= char < 32:
            return self.change_state(2)
        if 32 <= char < 33:
            return self.change_state(1, checkpoint=True)
        if 33 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

    def s2(self, char):
        if 0 <= char < 9:
            return self.change_state(2)
        if 9 <= char < 10:
            return self.change_state(2)
        if 10 <= char < 11:
            return self.change_state(2)
        if 11 <= char < 32:
            return self.change_state(2)
        if 32 <= char < 33:
            return self.change_state(2)
        if 33 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

    def s1(self, char):
        if 0 <= char < 9:
            return self.change_state(2)
        if 9 <= char < 10:
            return self.change_state(2)
        if 10 <= char < 11:
            return self.change_state(2)
        if 11 <= char < 32:
            return self.change_state(2)
        if 32 <= char < 33:
            return self.change_state(2)
        if 33 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

class NumberScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "number"

    def s0(self, char):
        if 0 <= char < 43:
            return self.change_state(6)
        if 43 <= char < 44:
            return self.change_state(6)
        if 44 <= char < 45:
            return self.change_state(6)
        if 45 <= char < 46:
            return self.change_state(6)
        if 46 <= char < 47:
            return self.change_state(6)
        if 47 <= char < 48:
            return self.change_state(6)
        if 48 <= char < 49:
            return self.change_state(3, checkpoint=True)
        if 49 <= char < 58:
            return self.change_state(8, checkpoint=True)
        if 58 <= char < 69:
            return self.change_state(6)
        if 69 <= char < 70:
            return self.change_state(6)
        if 70 <= char < 101:
            return self.change_state(6)
        if 101 <= char < 102:
            return self.change_state(6)
        if 102 <= char < 4294967295:
            return self.change_state(6)
        return self.change_state(-1)

    def s6(self, char):
        if 0 <= char < 43:
            return self.change_state(6)
        if 43 <= char < 44:
            return self.change_state(6)
        if 44 <= char < 45:
            return self.change_state(6)
        if 45 <= char < 46:
            return self.change_state(6)
        if 46 <= char < 47:
            return self.change_state(6)
        if 47 <= char < 48:
            return self.change_state(6)
        if 48 <= char < 49:
            return self.change_state(6)
        if 49 <= char < 58:
            return self.change_state(6)
        if 58 <= char < 69:
            return self.change_state(6)
        if 69 <= char < 70:
            return self.change_state(6)
        if 70 <= char < 101:
            return self.change_state(6)
        if 101 <= char < 102:
            return self.change_state(6)
        if 102 <= char < 4294967295:
            return self.change_state(6)
        return self.change_state(-1)

    def s3(self, char):
        if 0 <= char < 43:
            return self.change_state(6)
        if 43 <= char < 44:
            return self.change_state(6)
        if 44 <= char < 45:
            return self.change_state(6)
        if 45 <= char < 46:
            return self.change_state(6)
        if 46 <= char < 47:
            return self.change_state(7)
        if 47 <= char < 48:
            return self.change_state(6)
        if 48 <= char < 49:
            return self.change_state(6)
        if 49 <= char < 58:
            return self.change_state(6)
        if 58 <= char < 69:
            return self.change_state(6)
        if 69 <= char < 70:
            return self.change_state(5)
        if 70 <= char < 101:
            return self.change_state(6)
        if 101 <= char < 102:
            return self.change_state(5)
        if 102 <= char < 4294967295:
            return self.change_state(6)
        return self.change_state(-1)

    def s8(self, char):
        if 0 <= char < 43:
            return self.change_state(6)
        if 43 <= char < 44:
            return self.change_state(6)
        if 44 <= char < 45:
            return self.change_state(6)
        if 45 <= char < 46:
            return self.change_state(6)
        if 46 <= char < 47:
            return self.change_state(7)
        if 47 <= char < 48:
            return self.change_state(6)
        if 48 <= char < 49:
            return self.change_state(8, checkpoint=True)
        if 49 <= char < 58:
            return self.change_state(8, checkpoint=True)
        if 58 <= char < 69:
            return self.change_state(6)
        if 69 <= char < 70:
            return self.change_state(5)
        if 70 <= char < 101:
            return self.change_state(6)
        if 101 <= char < 102:
            return self.change_state(5)
        if 102 <= char < 4294967295:
            return self.change_state(6)
        return self.change_state(-1)

    def s1(self, char):
        if 0 <= char < 43:
            return self.change_state(6)
        if 43 <= char < 44:
            return self.change_state(6)
        if 44 <= char < 45:
            return self.change_state(6)
        if 45 <= char < 46:
            return self.change_state(6)
        if 46 <= char < 47:
            return self.change_state(6)
        if 47 <= char < 48:
            return self.change_state(6)
        if 48 <= char < 49:
            return self.change_state(6)
        if 49 <= char < 58:
            return self.change_state(6)
        if 58 <= char < 69:
            return self.change_state(6)
        if 69 <= char < 70:
            return self.change_state(6)
        if 70 <= char < 101:
            return self.change_state(6)
        if 101 <= char < 102:
            return self.change_state(6)
        if 102 <= char < 4294967295:
            return self.change_state(6)
        return self.change_state(-1)

    def s2(self, char):
        if 0 <= char < 43:
            return self.change_state(6)
        if 43 <= char < 44:
            return self.change_state(6)
        if 44 <= char < 45:
            return self.change_state(6)
        if 45 <= char < 46:
            return self.change_state(6)
        if 46 <= char < 47:
            return self.change_state(6)
        if 47 <= char < 48:
            return self.change_state(6)
        if 48 <= char < 49:
            return self.change_state(1, checkpoint=True)
        if 49 <= char < 58:
            return self.change_state(4, checkpoint=True)
        if 58 <= char < 69:
            return self.change_state(6)
        if 69 <= char < 70:
            return self.change_state(6)
        if 70 <= char < 101:
            return self.change_state(6)
        if 101 <= char < 102:
            return self.change_state(6)
        if 102 <= char < 4294967295:
            return self.change_state(6)
        return self.change_state(-1)

    def s4(self, char):
        if 0 <= char < 43:
            return self.change_state(6)
        if 43 <= char < 44:
            return self.change_state(6)
        if 44 <= char < 45:
            return self.change_state(6)
        if 45 <= char < 46:
            return self.change_state(6)
        if 46 <= char < 47:
            return self.change_state(6)
        if 47 <= char < 48:
            return self.change_state(6)
        if 48 <= char < 49:
            return self.change_state(4, checkpoint=True)
        if 49 <= char < 58:
            return self.change_state(4, checkpoint=True)
        if 58 <= char < 69:
            return self.change_state(6)
        if 69 <= char < 70:
            return self.change_state(6)
        if 70 <= char < 101:
            return self.change_state(6)
        if 101 <= char < 102:
            return self.change_state(6)
        if 102 <= char < 4294967295:
            return self.change_state(6)
        return self.change_state(-1)

    def s7(self, char):
        if 0 <= char < 43:
            return self.change_state(6)
        if 43 <= char < 44:
            return self.change_state(6)
        if 44 <= char < 45:
            return self.change_state(6)
        if 45 <= char < 46:
            return self.change_state(6)
        if 46 <= char < 47:
            return self.change_state(6)
        if 47 <= char < 48:
            return self.change_state(6)
        if 48 <= char < 49:
            return self.change_state(9, checkpoint=True)
        if 49 <= char < 58:
            return self.change_state(9, checkpoint=True)
        if 58 <= char < 69:
            return self.change_state(6)
        if 69 <= char < 70:
            return self.change_state(6)
        if 70 <= char < 101:
            return self.change_state(6)
        if 101 <= char < 102:
            return self.change_state(6)
        if 102 <= char < 4294967295:
            return self.change_state(6)
        return self.change_state(-1)

    def s5(self, char):
        if 0 <= char < 43:
            return self.change_state(6)
        if 43 <= char < 44:
            return self.change_state(2)
        if 44 <= char < 45:
            return self.change_state(6)
        if 45 <= char < 46:
            return self.change_state(2)
        if 46 <= char < 47:
            return self.change_state(6)
        if 47 <= char < 48:
            return self.change_state(6)
        if 48 <= char < 49:
            return self.change_state(1, checkpoint=True)
        if 49 <= char < 58:
            return self.change_state(4, checkpoint=True)
        if 58 <= char < 69:
            return self.change_state(6)
        if 69 <= char < 70:
            return self.change_state(6)
        if 70 <= char < 101:
            return self.change_state(6)
        if 101 <= char < 102:
            return self.change_state(6)
        if 102 <= char < 4294967295:
            return self.change_state(6)
        return self.change_state(-1)

    def s9(self, char):
        if 0 <= char < 43:
            return self.change_state(6)
        if 43 <= char < 44:
            return self.change_state(6)
        if 44 <= char < 45:
            return self.change_state(6)
        if 45 <= char < 46:
            return self.change_state(6)
        if 46 <= char < 47:
            return self.change_state(6)
        if 47 <= char < 48:
            return self.change_state(6)
        if 48 <= char < 49:
            return self.change_state(9, checkpoint=True)
        if 49 <= char < 58:
            return self.change_state(9, checkpoint=True)
        if 58 <= char < 69:
            return self.change_state(6)
        if 69 <= char < 70:
            return self.change_state(5)
        if 70 <= char < 101:
            return self.change_state(6)
        if 101 <= char < 102:
            return self.change_state(5)
        if 102 <= char < 4294967295:
            return self.change_state(6)
        return self.change_state(-1)

class CharacterScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "character"

    def s0(self, char):
        if 0 <= char < 32:
            return self.change_state(5)
        if 32 <= char < 39:
            return self.change_state(5)
        if 39 <= char < 40:
            return self.change_state(3)
        if 40 <= char < 92:
            return self.change_state(5)
        if 92 <= char < 93:
            return self.change_state(5)
        if 93 <= char < 110:
            return self.change_state(5)
        if 110 <= char < 111:
            return self.change_state(5)
        if 111 <= char < 116:
            return self.change_state(5)
        if 116 <= char < 117:
            return self.change_state(5)
        if 117 <= char < 127:
            return self.change_state(5)
        if 127 <= char < 4294967295:
            return self.change_state(5)
        return self.change_state(-1)

    def s5(self, char):
        if 0 <= char < 32:
            return self.change_state(5)
        if 32 <= char < 39:
            return self.change_state(5)
        if 39 <= char < 40:
            return self.change_state(5)
        if 40 <= char < 92:
            return self.change_state(5)
        if 92 <= char < 93:
            return self.change_state(5)
        if 93 <= char < 110:
            return self.change_state(5)
        if 110 <= char < 111:
            return self.change_state(5)
        if 111 <= char < 116:
            return self.change_state(5)
        if 116 <= char < 117:
            return self.change_state(5)
        if 117 <= char < 127:
            return self.change_state(5)
        if 127 <= char < 4294967295:
            return self.change_state(5)
        return self.change_state(-1)

    def s3(self, char):
        if 0 <= char < 32:
            return self.change_state(5)
        if 32 <= char < 39:
            return self.change_state(2)
        if 39 <= char < 40:
            return self.change_state(5)
        if 40 <= char < 92:
            return self.change_state(2)
        if 92 <= char < 93:
            return self.change_state(4)
        if 93 <= char < 110:
            return self.change_state(2)
        if 110 <= char < 111:
            return self.change_state(2)
        if 111 <= char < 116:
            return self.change_state(2)
        if 116 <= char < 117:
            return self.change_state(2)
        if 117 <= char < 127:
            return self.change_state(2)
        if 127 <= char < 4294967295:
            return self.change_state(5)
        return self.change_state(-1)

    def s1(self, char):
        if 0 <= char < 32:
            return self.change_state(5)
        if 32 <= char < 39:
            return self.change_state(5)
        if 39 <= char < 40:
            return self.change_state(5)
        if 40 <= char < 92:
            return self.change_state(5)
        if 92 <= char < 93:
            return self.change_state(5)
        if 93 <= char < 110:
            return self.change_state(5)
        if 110 <= char < 111:
            return self.change_state(5)
        if 111 <= char < 116:
            return self.change_state(5)
        if 116 <= char < 117:
            return self.change_state(5)
        if 117 <= char < 127:
            return self.change_state(5)
        if 127 <= char < 4294967295:
            return self.change_state(5)
        return self.change_state(-1)

    def s2(self, char):
        if 0 <= char < 32:
            return self.change_state(5)
        if 32 <= char < 39:
            return self.change_state(5)
        if 39 <= char < 40:
            return self.change_state(1, checkpoint=True)
        if 40 <= char < 92:
            return self.change_state(5)
        if 92 <= char < 93:
            return self.change_state(5)
        if 93 <= char < 110:
            return self.change_state(5)
        if 110 <= char < 111:
            return self.change_state(5)
        if 111 <= char < 116:
            return self.change_state(5)
        if 116 <= char < 117:
            return self.change_state(5)
        if 117 <= char < 127:
            return self.change_state(5)
        if 127 <= char < 4294967295:
            return self.change_state(5)
        return self.change_state(-1)

    def s4(self, char):
        if 0 <= char < 32:
            return self.change_state(5)
        if 32 <= char < 39:
            return self.change_state(5)
        if 39 <= char < 40:
            return self.change_state(2)
        if 40 <= char < 92:
            return self.change_state(5)
        if 92 <= char < 93:
            return self.change_state(2)
        if 93 <= char < 110:
            return self.change_state(5)
        if 110 <= char < 111:
            return self.change_state(2)
        if 111 <= char < 116:
            return self.change_state(5)
        if 116 <= char < 117:
            return self.change_state(2)
        if 117 <= char < 127:
            return self.change_state(5)
        if 127 <= char < 4294967295:
            return self.change_state(5)
        return self.change_state(-1)

class StringScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "string"

    def s0(self, char):
        if 0 <= char < 32:
            return self.change_state(4)
        if 32 <= char < 34:
            return self.change_state(4)
        if 34 <= char < 35:
            return self.change_state(2)
        if 35 <= char < 92:
            return self.change_state(4)
        if 92 <= char < 93:
            return self.change_state(4)
        if 93 <= char < 127:
            return self.change_state(4)
        if 127 <= char < 4294967295:
            return self.change_state(4)
        return self.change_state(-1)

    def s4(self, char):
        if 0 <= char < 32:
            return self.change_state(4)
        if 32 <= char < 34:
            return self.change_state(4)
        if 34 <= char < 35:
            return self.change_state(4)
        if 35 <= char < 92:
            return self.change_state(4)
        if 92 <= char < 93:
            return self.change_state(4)
        if 93 <= char < 127:
            return self.change_state(4)
        if 127 <= char < 4294967295:
            return self.change_state(4)
        return self.change_state(-1)

    def s2(self, char):
        if 0 <= char < 32:
            return self.change_state(4)
        if 32 <= char < 34:
            return self.change_state(2)
        if 34 <= char < 35:
            return self.change_state(1, checkpoint=True)
        if 35 <= char < 92:
            return self.change_state(2)
        if 92 <= char < 93:
            return self.change_state(3)
        if 93 <= char < 127:
            return self.change_state(2)
        if 127 <= char < 4294967295:
            return self.change_state(4)
        return self.change_state(-1)

    def s1(self, char):
        if 0 <= char < 32:
            return self.change_state(4)
        if 32 <= char < 34:
            return self.change_state(4)
        if 34 <= char < 35:
            return self.change_state(4)
        if 35 <= char < 92:
            return self.change_state(4)
        if 92 <= char < 93:
            return self.change_state(4)
        if 93 <= char < 127:
            return self.change_state(4)
        if 127 <= char < 4294967295:
            return self.change_state(4)
        return self.change_state(-1)

    def s3(self, char):
        if 0 <= char < 32:
            return self.change_state(4)
        if 32 <= char < 34:
            return self.change_state(2)
        if 34 <= char < 35:
            return self.change_state(2)
        if 35 <= char < 92:
            return self.change_state(2)
        if 92 <= char < 93:
            return self.change_state(2)
        if 93 <= char < 127:
            return self.change_state(2)
        if 127 <= char < 4294967295:
            return self.change_state(4)
        return self.change_state(-1)

class DotScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "dot"

    def s0(self, char):
        if 0 <= char < 46:
            return self.change_state(2)
        if 46 <= char < 47:
            return self.change_state(1, checkpoint=True)
        if 47 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

    def s2(self, char):
        if 0 <= char < 46:
            return self.change_state(2)
        if 46 <= char < 47:
            return self.change_state(2)
        if 47 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

    def s1(self, char):
        if 0 <= char < 46:
            return self.change_state(2)
        if 46 <= char < 47:
            return self.change_state(2)
        if 47 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

class LparenScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "lparen"

    def s0(self, char):
        if 0 <= char < 40:
            return self.change_state(2)
        if 40 <= char < 41:
            return self.change_state(1, checkpoint=True)
        if 41 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

    def s2(self, char):
        if 0 <= char < 40:
            return self.change_state(2)
        if 40 <= char < 41:
            return self.change_state(2)
        if 41 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

    def s1(self, char):
        if 0 <= char < 40:
            return self.change_state(2)
        if 40 <= char < 41:
            return self.change_state(2)
        if 41 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

class RparenScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "rparen"

    def s0(self, char):
        if 0 <= char < 41:
            return self.change_state(2)
        if 41 <= char < 42:
            return self.change_state(1, checkpoint=True)
        if 42 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

    def s2(self, char):
        if 0 <= char < 41:
            return self.change_state(2)
        if 41 <= char < 42:
            return self.change_state(2)
        if 42 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

    def s1(self, char):
        if 0 <= char < 41:
            return self.change_state(2)
        if 41 <= char < 42:
            return self.change_state(2)
        if 42 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

class CommaScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "comma"

    def s0(self, char):
        if 0 <= char < 44:
            return self.change_state(2)
        if 44 <= char < 45:
            return self.change_state(1, checkpoint=True)
        if 45 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

    def s2(self, char):
        if 0 <= char < 44:
            return self.change_state(2)
        if 44 <= char < 45:
            return self.change_state(2)
        if 45 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

    def s1(self, char):
        if 0 <= char < 44:
            return self.change_state(2)
        if 44 <= char < 45:
            return self.change_state(2)
        if 45 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

class StarScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "star"

    def s0(self, char):
        if 0 <= char < 42:
            return self.change_state(2)
        if 42 <= char < 43:
            return self.change_state(1, checkpoint=True)
        if 43 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

    def s2(self, char):
        if 0 <= char < 42:
            return self.change_state(2)
        if 42 <= char < 43:
            return self.change_state(2)
        if 43 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

    def s1(self, char):
        if 0 <= char < 42:
            return self.change_state(2)
        if 42 <= char < 43:
            return self.change_state(2)
        if 43 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

class EqualScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "equal"

    def s0(self, char):
        if 0 <= char < 61:
            return self.change_state(2)
        if 61 <= char < 62:
            return self.change_state(1, checkpoint=True)
        if 62 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

    def s2(self, char):
        if 0 <= char < 61:
            return self.change_state(2)
        if 61 <= char < 62:
            return self.change_state(2)
        if 62 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

    def s1(self, char):
        if 0 <= char < 61:
            return self.change_state(2)
        if 61 <= char < 62:
            return self.change_state(2)
        if 62 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

class LbraceScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "lbrace"

    def s0(self, char):
        if 0 <= char < 123:
            return self.change_state(2)
        if 123 <= char < 124:
            return self.change_state(1, checkpoint=True)
        if 124 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

    def s2(self, char):
        if 0 <= char < 123:
            return self.change_state(2)
        if 123 <= char < 124:
            return self.change_state(2)
        if 124 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

    def s1(self, char):
        if 0 <= char < 123:
            return self.change_state(2)
        if 123 <= char < 124:
            return self.change_state(2)
        if 124 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

class RbraceScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "rbrace"

    def s0(self, char):
        if 0 <= char < 125:
            return self.change_state(2)
        if 125 <= char < 126:
            return self.change_state(1, checkpoint=True)
        if 126 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

    def s2(self, char):
        if 0 <= char < 125:
            return self.change_state(2)
        if 125 <= char < 126:
            return self.change_state(2)
        if 126 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

    def s1(self, char):
        if 0 <= char < 125:
            return self.change_state(2)
        if 125 <= char < 126:
            return self.change_state(2)
        if 126 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

class ColonScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "colon"

    def s0(self, char):
        if 0 <= char < 58:
            return self.change_state(2)
        if 58 <= char < 59:
            return self.change_state(1, checkpoint=True)
        if 59 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

    def s2(self, char):
        if 0 <= char < 58:
            return self.change_state(2)
        if 58 <= char < 59:
            return self.change_state(2)
        if 59 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

    def s1(self, char):
        if 0 <= char < 58:
            return self.change_state(2)
        if 58 <= char < 59:
            return self.change_state(2)
        if 59 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

class LbracketScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "lbracket"

    def s0(self, char):
        if 0 <= char < 91:
            return self.change_state(2)
        if 91 <= char < 92:
            return self.change_state(1, checkpoint=True)
        if 92 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

    def s2(self, char):
        if 0 <= char < 91:
            return self.change_state(2)
        if 91 <= char < 92:
            return self.change_state(2)
        if 92 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

    def s1(self, char):
        if 0 <= char < 91:
            return self.change_state(2)
        if 91 <= char < 92:
            return self.change_state(2)
        if 92 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

class RbracketScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "rbracket"

    def s0(self, char):
        if 0 <= char < 93:
            return self.change_state(2)
        if 93 <= char < 94:
            return self.change_state(1, checkpoint=True)
        if 94 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

    def s2(self, char):
        if 0 <= char < 93:
            return self.change_state(2)
        if 93 <= char < 94:
            return self.change_state(2)
        if 94 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

    def s1(self, char):
        if 0 <= char < 93:
            return self.change_state(2)
        if 93 <= char < 94:
            return self.change_state(2)
        if 94 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

class PlusScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "plus"

    def s0(self, char):
        if 0 <= char < 43:
            return self.change_state(2)
        if 43 <= char < 44:
            return self.change_state(1, checkpoint=True)
        if 44 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

    def s2(self, char):
        if 0 <= char < 43:
            return self.change_state(2)
        if 43 <= char < 44:
            return self.change_state(2)
        if 44 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

    def s1(self, char):
        if 0 <= char < 43:
            return self.change_state(2)
        if 43 <= char < 44:
            return self.change_state(2)
        if 44 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

class MinusScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "minus"

    def s0(self, char):
        if 0 <= char < 45:
            return self.change_state(2)
        if 45 <= char < 46:
            return self.change_state(1, checkpoint=True)
        if 46 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

    def s2(self, char):
        if 0 <= char < 45:
            return self.change_state(2)
        if 45 <= char < 46:
            return self.change_state(2)
        if 46 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

    def s1(self, char):
        if 0 <= char < 45:
            return self.change_state(2)
        if 45 <= char < 46:
            return self.change_state(2)
        if 46 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

class SlashScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "slash"

    def s0(self, char):
        if 0 <= char < 47:
            return self.change_state(2)
        if 47 <= char < 48:
            return self.change_state(1, checkpoint=True)
        if 48 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

    def s2(self, char):
        if 0 <= char < 47:
            return self.change_state(2)
        if 47 <= char < 48:
            return self.change_state(2)
        if 48 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

    def s1(self, char):
        if 0 <= char < 47:
            return self.change_state(2)
        if 47 <= char < 48:
            return self.change_state(2)
        if 48 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

class LessthanScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "lessthan"

    def s0(self, char):
        if 0 <= char < 60:
            return self.change_state(2)
        if 60 <= char < 61:
            return self.change_state(1, checkpoint=True)
        if 61 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

    def s2(self, char):
        if 0 <= char < 60:
            return self.change_state(2)
        if 60 <= char < 61:
            return self.change_state(2)
        if 61 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

    def s1(self, char):
        if 0 <= char < 60:
            return self.change_state(2)
        if 60 <= char < 61:
            return self.change_state(2)
        if 61 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

class GreaterthanScanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "greaterthan"

    def s0(self, char):
        if 0 <= char < 62:
            return self.change_state(2)
        if 62 <= char < 63:
            return self.change_state(1, checkpoint=True)
        if 63 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

    def s2(self, char):
        if 0 <= char < 62:
            return self.change_state(2)
        if 62 <= char < 63:
            return self.change_state(2)
        if 63 <= char < 4294967295:
            return self.change_state(2)
        return self.change_state(-1)

    def s1(self, char):
        if 0 <= char < 62:
            return self.change_state(2)
        if 62 <= char < 63:
            return self.change_state(2)
        if 63 <= char < 4294967295:
            return self.change_state(2)
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
