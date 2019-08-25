class TransitionError(Exception):
    pass

class Scanner:
    """Base class for scangen-generated scanners."""

    def __init__(self):
        self.state = 0
        self.checkpoint = 0
        self.token = ""

    def next(self, char):
        """State transition on input char, returns True if successful."""
        self.checkpoint = self.state
        try:
            return getattr(self, f"s{self.state}")(ord(char))
        except:
            self.state = -1
            return False

    def backtrack(self):
        """Undo most previous transition, can only undo one step."""
        self.state = self.checkpoint
        self.checkpoint = -1
        return self.state != -1

    def __repr__(self):
        return f"<Scanner {self.token} state:{self.state} checkpoint:{self.checkpoint}>"

    def accepts(self):
        raise NotImplementedError

## for scanner in scanners
## include "scanner.py"
## endfor
