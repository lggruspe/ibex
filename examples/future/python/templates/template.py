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

## for scanner in scanners
## include "scanner.py"
## endfor

SCANNERS = {
## for scanner in scanners
    "{{ scanner.token }}": {{ scanner.token|title }}Scanner,
## endfor
}
