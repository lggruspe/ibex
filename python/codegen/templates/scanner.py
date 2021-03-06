import sys

class Token:
    EMPTY = "empty"
    {%- for recognizer in scanner %}
    {{ recognizer.token|upper }} = "{{ recognizer.token|lower }}"
    {%- endfor %}

class InputStack:
    def __init__(self, file=sys.stdin):
        self.stack = []
        self.file = file
        self.done = False

    def get(self):
        if self.stack:
            return self.stack.pop()
        if self.done:
            return ""
        a = self.file.read(1)
        if not a:
            self.done = True
        return a

    def unget(self, a):
        if not a:
            raise Exception
        self.stack.append(a)

class BaseRecognizer:
    def __init__(self, token=Token.EMPTY, accept=False, error=-1):
        self.token = token
        self.accept = accept
        self.error = error

    def next(self, q, a):
        raise NotImplementedError

    def match(self, io=InputStack()):
        checkpoint = [0]
        lexeme = []
        while checkpoint[-1] != self.error and (a := io.get()):
            status, r = self.next(checkpoint[-1], ord(a))
            if status == 1:
                self.accept = True
                checkpoint.clear()
            checkpoint.append(r)
            lexeme.append(a)
        for _ in range(len(checkpoint), 1, -1):
            io.unget(lexeme.pop())
        return self.accept, "".join(lexeme)

{% for recognizer in scanner %}
{% include "recognizer.py" %}
{% endfor %}
def match_longest(*recs, io=InputStack()):
    token = Token.EMPTY
    lexeme = ""
    for T in recs:
        r = T()
        ok, s = r.match(io)
        if ok and len(s) > len(lexeme):
            token = r.token
            lexeme = s
        for a in s[-1::-1]:
            io.unget(a)
    for _ in lexeme:
        io.get()
    return token, lexeme

SCANNER = {
{%- for recognizer in scanner %}
    "{{ recognizer.token }}": {{ recognizer.token|title }},
{%- endfor %}
}
