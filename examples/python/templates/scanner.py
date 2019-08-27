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
            self.state = {{ next_state }}
## endfor
## if scanner.transitions[state]|length == 0
        raise TransitionError

## else
        else:
            raise TransitionError

## endif
## endfor
