class {{ scanner.token|title }}Scanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "{{ scanner.token }}"

## for state in scanner.transitions

    def s{{ state }}(self, char):
## for transition in scanner.transitions[state]
## set start = transition[0].start
## set end = transition[0].end
## set next_state = transition[1]
        if {{ start }} <= char <= {{ end }}:
            self.state = {{ next_state }}
            return self.state
## endfor
        raise TransitionError

## endfor
