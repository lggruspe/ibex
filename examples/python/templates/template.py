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

## for scanner in scanners
## include "scanner.py"
## endfor
