import sys

class Token:
    EMPTY = "empty"
    {%- for scanner in scanners %}
    {{ scanner.token|upper }} = "{{ scanner.token|lower }}"
    {%- endfor %}

class InputStack:
    def __init__(self):
        self.stack = []

    def readfile(self, file=sys.stdin):
        input_ = file.read()
        self.stack = [a for a in input_[::-1]]

    def get(self):
        if self.stack:
            return self.stack.pop()
        return ""

    def unget(self, a):
        if not a:
            raise Exception
        self.stack.append(a)

class BaseRecognizer:
    def __init__(self, token=Token.EMPTY, accept=False, error=-1,
            io=InputStack()):
        self.token = token
        self.accept = accept
        self.error = error
        self.io = io

    def next(self, q, a):
        raise NotImplementedError

    def match(self):
        checkpoint = [0]
        lexeme = []
        while checkpoint[-1] != self.error:
            a = self.io.get()
            if not a:
                break
            status, r = self.next(checkpoint[-1], ord(a))
            if status == 1:
                self.accept = True
                checkpoint.clear()
            checkpoint.append(r)
            lexeme.append(a)
        for _ in checkpoint[-1:0:-1]:
            self.io.unget(lexeme.pop())
        return self.accept, "".join(lexeme)

{% for scanner in scanners %}
{% include "scanner.py" %}
{% endfor %}
def match_first(*recs, io=None):
    if not io:
        io = InputStack()
        io.readfile(sys.stdin)
    for T in recs:
        r = T(io)
        ok, s = r.match()
        if ok:
            return r.token, s
    return Token.EMPTY, ""

def match_longest(*recs, io=None):
    if not io:
        io = InputStack()
        io.readfile(sys.stdin)
    token = Token.EMPTY
    lexeme = ""
    for T in recs:
        r = T(io)
        ok, s = r.match()
        if ok and len(s) > len(lexeme):
            token = r.token
            lexeme = s
        for a in s[-1::-1]:
            io.unget(a)
    for _ in lexeme:
        io.get()
    return token, lexeme

SCANNERS = {
{%- for scanner in scanners %}
    "{{ scanner.token }}": {{ scanner.token|title }},
{%- endfor %}
}
