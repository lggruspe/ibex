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
## if loop.index == 1
        if char == {{ start }}:
## else
        elif char == {{ start }}:
## endif
## else
## if loop.index == 1
        if {{ start }} <= char <= {{ end }}:
## else
        elif {{ start }} <= char <= {{ end }}:
## endif
## endif
## if next_state in scanner.accepts
            self.change_state({{ next_state }}, checkpoint=True)
## else
            self.change_state({{ next_state }})
## endif
## endfor
## if scanner.transitions[state]|length == 0
        raise TransitionError

## else
        else:
            raise TransitionError

## endif
## endfor
