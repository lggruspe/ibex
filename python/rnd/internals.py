"""Direct wrapper for libcrnd."""

import ctypes

crnd = ctypes.CDLL("libcrnd.so")

class CInterval(ctypes.Structure):
    """Wrapper for struct rnd_symbol_internal."""

    _fields_ = [
        ("start", ctypes.c_int),
        ("end", ctypes.c_int)
    ]

class CExpr(ctypes.Structure):
    """Wrapper for struct rnd_expr."""

CExpr._fields_ = [
    ("type", ctypes.c_char),
    ("left", ctypes.POINTER(CExpr)),
    ("right", ctypes.POINTER(CExpr)),
    ("symbols", CInterval)
]

class CTransition(ctypes.Structure):
    """Wrapper for struct rnd_transition."""

    _fields_ = [
        ("current_state", ctypes.c_int),
        ("next_state", ctypes.c_int),
        ("symbols", CInterval)
    ]

class CDfa(ctypes.Structure):
    """Wrapper for struct rnd_dfa."""

    _fields_ = [
        ("number_states", ctypes.c_size_t),
        ("number_transitions", ctypes.c_size_t),
        ("number_accept_states", ctypes.c_size_t),
        ("start_state", ctypes.c_int),
        ("transitions", ctypes.POINTER(CTransition)),
        ("accept_states", ctypes.POINTER(ctypes.c_int)),
        ("error", ctypes.c_char_p)
    ]

crnd.rnd_convert.argtypes = [ctypes.POINTER(CExpr)]
crnd.rnd_convert.restype = CDfa

crnd.rnd_dfa_destroy.argtypes = [ctypes.POINTER(CDfa)]
crnd.rnd_dfa_destroy.restype = None

crnd.rnd_expr_symbol.argtypes = [ctypes.c_int, ctypes.c_int]
crnd.rnd_expr_symbol.restype = ctypes.POINTER(CExpr)

crnd.rnd_expr_union.argtypes = [ctypes.POINTER(CExpr), ctypes.POINTER(CExpr)]
crnd.rnd_expr_union.restype = ctypes.POINTER(CExpr)

crnd.rnd_expr_concatenation.argtypes = [ctypes.POINTER(CExpr), ctypes.POINTER(CExpr)]
crnd.rnd_expr_concatenation.restype = ctypes.POINTER(CExpr)

crnd.rnd_expr_closure.argtypes = [ctypes.POINTER(CExpr)]
crnd.rnd_expr_closure.restype = ctypes.POINTER(CExpr)

crnd.rnd_expr_destroy.argtypes = [ctypes.POINTER(CExpr)]
crnd.rnd_expr_destroy.restype = None

crnd.rnd_expr_free.argtypes = [ctypes.POINTER(CExpr)]
crnd.rnd_expr_free.restype = None

crnd.rnd_get_expr_counter.restype = ctypes.c_int
