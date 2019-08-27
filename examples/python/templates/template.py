class TransitionError(Exception):
    pass

class Scanner:
    """Base class for scangen-generated scanners."""

    def __init__(self):
        self.checkpoint = [0]
        self.token = ""
        self._accepts = False

    @property
    def state(self):
        return self.checkpoint[-1] if self.checkpoint else -1

    def change_state(self, next_state, checkpoint=False):
        """Transition and set checkpoint if visiting an accept state."""
        if checkpoint:
            self.checkpoint = []
            self._accepts = True
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

    def accepts(self):
        """Returns true if the scanner passes an accept state even once."""
        return self._accepts

## for scanner in scanners
## include "scanner.py"
## endfor

SCANNERS = {
## for scanner in scanners
    "{{ scanner.token }}": {{ scanner.token|title }}Scanner,
## endfor
}
