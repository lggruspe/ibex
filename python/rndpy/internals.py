import ctypes

crnd = ctypes.CDLL("libcrnd.so")

class CInterval(ctypes.Structure):
    _fields_ = [
        ("start", ctypes.c_int),
        ("end", ctypes.c_int)
    ]

class CExpr(ctypes.Structure):
    pass
CExpr._fields_ = [
    ("type", ctypes.c_char),
    ("left", ctypes.POINTER(CExpr)),
    ("right", ctypes.POINTER(CExpr)),
    ("symbols", CInterval)
]

class CTransition(ctypes.Structure):
    _fields_ = [
        ("current_state", ctypes.c_int),
        ("next_state", ctypes.c_int),
        ("symbols", CInterval)
    ]

class CDfa(ctypes.Structure):
    _fields_ = [
        ("number_states", ctypes.c_size_t),
        ("number_transitions", ctypes.c_size_t),
        ("number_accept_states", ctypes.c_size_t),
        ("start_state", ctypes.c_int),
        ("transitions", ctypes.POINTER(CTransition)),
        ("accept_states", ctypes.POINTER(ctypes.c_int)),
        ("error", ctypes.c_char_p)
    ]

crnd_convert = crnd.rnd_convert
crnd_convert.argtypes = [ctypes.POINTER(CExpr)]
crnd_convert.restype = CDfa

crnd_dfa_destroy = crnd.rnd_dfa_destroy
crnd_dfa_destroy.argtypes = [ctypes.POINTER(CDfa)]
crnd_dfa_destroy.restype = None

crnd_expr_symbol = crnd.rnd_expr_symbol
crnd_expr_symbol.argtypes = [ctypes.c_int, ctypes.c_int]
crnd_expr_symbol.restype = ctypes.POINTER(CExpr)

crnd_expr_union = crnd.rnd_expr_union
crnd_expr_union.argtypes = [ctypes.POINTER(CExpr), ctypes.POINTER(CExpr)]
crnd_expr_union.restype = ctypes.POINTER(CExpr)

crnd_expr_concatenation = crnd.rnd_expr_concatenation
crnd_expr_concatenation.argtypes = [ctypes.POINTER(CExpr), ctypes.POINTER(CExpr)]
crnd_expr_concatenation.restype = ctypes.POINTER(CExpr)

crnd_expr_closure = crnd.rnd_expr_closure
crnd_expr_closure.argtypes = [ctypes.POINTER(CExpr)]
crnd_expr_closure.restype = ctypes.POINTER(CExpr)

crnd_expr_destroy = crnd.rnd_expr_destroy
crnd_expr_destroy.argtypes = [ctypes.POINTER(CExpr)]
crnd_expr_destroy.restype = None

crnd_expr_free = crnd.rnd_expr_free
crnd_expr_free.argtypes = [ctypes.POINTER(CExpr)]
crnd_expr_free.restype = None

crnd_get_expr_counter = crnd.rnd_get_expr_counter
crnd_get_expr_counter.restype = ctypes.c_int


