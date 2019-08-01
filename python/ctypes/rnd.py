import ctypes
import enum

crnd = ctypes.CDLL("libcrnd.so")

class Interval(ctypes.Structure):
    _fields_ = [
            ("start", ctypes.c_int),
            ("end", ctypes.c_int)
            ]

class Expr(ctypes.Structure):
    pass
Expr._fields_ = [
        ("type", ctypes.c_int),
        ("left", ctypes.POINTER(Expr)),
        ("right", ctypes.POINTER(Expr)),
        ("symbols", Interval)
        ]

class Transition(ctypes.Structure):
    _fields_ = [
            ("current_state", ctypes.c_int),
            ("next_state", ctypes.c_int),
            ("symbols", Interval)
            ]

# TODO is c_int the correct type?
# TODO update c bindings?
class Dfa(ctypes.Structure):
    _fields_ = [
            ("number_states", ctypes.c_int),
            ("number_transitions", ctypes.c_int),
            ("number_accept_states", ctypes.c_int),
            ("start_state", ctypes.c_int),
            ("transitions", ctypes.POINTER(Transition)),
            ("accept_states", ctypes.POINTER(ctypes.c_int)),
            ("error", ctypes.c_char_p)
            ]

def convert(expr):
    rnd_convert = crnd.rnd_convert
    rnd_convert.argtypes = [ctypes.POINTER(Expr)]
    rnd_convert.restype = Dfa
    # byref(expr)

def destroy_dfa(dfa):
    f = crnd.rnd_dfa_destroy
    f.argtypes = [ctypes.POINTER(Dfa)]
    f.restype = None

def symbol(start, end):
    f = crnd.rnd_expr_symbol
    f.argtypes = [ctypes.c_int, ctypes.c_int]
    f.restype = ctypes.POINTER(Expr)

def union(left, right):
    f = crnd.rnd_expr_union
    f.argtypes = [ctypes.POINTER(Expr), ctypes.POINTER(Expr)]
    f.restype = ctypes.POINTER(Expr)

def concatenation(left, right):
    f = crnd.rnd_expr_concatenation
    f.argtypes = [ctypes.POINTER(Expr), ctypes.POINTER(Expr)]
    f.restype = ctypes.POINTER(Expr)

def closure(left):
    f = crnd.rnd_expr_closure
    f.argtypes = [ctypes.POINTER(Expr)]
    f.restype = ctypes.POINTER(Expr)

def destroy_expr(expr):
    f = crnd.rnd_expr_destroy
    f.argtypes = [ctypes.POINTER(Expr)]
    f.restype = None


# Python bindings

class ExprType(enum.Enum):
    SYMBOL = enum.auto()
    UNION = enum.auto()
    CONCATENATION = enum.auto()
    CLOSURE = enum.auto()
