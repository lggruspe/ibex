class {{ scanner.token|title }}Scanner(Scanner):
    def __init__(self):
        super().__init__("{{ scanner.token }}")

## for state in scanner.transitions
    def s{{ state }}(self, char):
        if not char:
            return self.fail()
## for transition in scanner.transitions[state]
## set start = transition[0].start
## set end = transition[0].end
## set next_state = transition[1]
        elif {{ start }} <= char <= {{ end }}:
            self.state = {{ next_state }}
            return self.state
## endfor
        return self.fail()

## endfor
