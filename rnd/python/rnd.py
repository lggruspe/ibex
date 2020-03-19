import ctypes
import json

librnd = ctypes.CDLL("librnd.so")
librnd.rnd_open.argtypes = [ctypes.c_char_p]
librnd.rnd_open.restype = ctypes.POINTER(ctypes.c_char)
librnd.rnd_close.argtypes = [ctypes.c_void_p]
librnd.rnd_close.restype = None

class DFA:
    def __init__(self, s):
        self.regex = s
        re = librnd.rnd_open(s.encode("utf-8"))
        js = json.loads(ctypes.cast(re, ctypes.c_char_p).value)
        librnd.rnd_close(re)
        self.start = js["start"]
        self.accepts = set(js["accepts"])
        self.transitions = {}
        for q, dq in enumerate(js["transitions"]):
            self.transitions[q] = dq[:]
        self.error = js["error"]

def from_class(cls):
    """Does not include token names that start with _."""
    keys = [k for k in cls.__dict__.keys() if k[0] != '_']
    return {k:cls.__dict__[k] for k in keys}

def convert(tokens):
    scanner = []
    for k, v in tokens.items():
        dfa = DFA(v)
        dfa.token = k
        scanner.append(dfa)
    return scanner
