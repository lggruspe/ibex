class {{ scanner.token|title }}Scanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "{{ scanner.token }}"
## if 0 in scanner.accepts
        self.accept = True

## else
        self.accept = False

## endif

## for state in scanner.transitions

    def s{{ state }}(self, char):
## for transition in scanner.transitions[state]
## set start = transition[0].start
## set end = transition[0].end
## set next_state = transition[1]
        if {{ start }} <= char <= {{ end }}:
            self.state = {{ next_state }}
## if next_state in scanner.accepts
            self.accept = True
## else
            self.accept = False
## endif
            return self.state
## endfor
        self.accept = False
        raise TransitionError

## endfor
