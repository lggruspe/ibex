import ctypes
import json

libsagl = ctypes.CDLL("libsagl.so")
libsagl.grammarp_open.argtypes = [ctypes.c_char_p]
libsagl.grammarp_open.restype = ctypes.POINTER(ctypes.c_char)
libsagl.grammarp_close.argtypes = [ctypes.c_void_p]
libsagl.grammarp_close.restype = None

def parse_grammar(bnf):
    s = libsagl.grammarp_open(bnf.encode("utf-8"))
    m = json.loads(ctypes.cast(s, ctypes.c_char_p).value)
    libsagl.grammarp_close(s)
    return m
