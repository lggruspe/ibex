class {{ scanner.token|title }}Scanner(Scanner):
    def __init__(self):
        super().__init__()
        self.token = "{{ scanner.token }}"

    def accepts(self):
        return self.state in [{{ scanner.accepts | join(", ") }}]

## for state in scanner.transitions

    def s{{ state }}(self, char):
## for transition in scanner.transitions[state]
## set start = transition[0].start
## set end = transition[0].end
## set next_state = transition[1]
## if start == end
        if char == {{ start }}:
## else
        if {{ start }} <= char <= {{ end }}:
## endif
            self.state = {{ next_state }}
            return self.state
## endfor
        raise TransitionError

## endfor
