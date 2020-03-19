import ctypes
import json

libsagl = ctypes.CDLL("libsagl.so")
libsagl.sagl_open.argtypes = [ctypes.c_char_p]
libsagl.sagl_open.restype = ctypes.POINTER(ctypes.c_char)
libsagl.sagl_close.argtypes = [ctypes.c_void_p]
libsagl.sagl_close.restype = None

def parse_grammar(bnf):
    s = libsagl.sagl_open(bnf.encode("utf-8"))
    m = json.loads(ctypes.cast(s, ctypes.c_char_p).value)
    libsagl.sagl_close(s)
    return m
