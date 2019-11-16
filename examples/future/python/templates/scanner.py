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
## if start == end
        if char == {{ start }}:
## else
        if {{ start }} <= char <= {{ end }}:
## endif
## if next_state in scanner.accepts
            return self.change_state({{ next_state }}, checkpoint=True)
## else
            return self.change_state({{ next_state }})
## endif
## endfor
        return self.change_state(-1)

## endfor
