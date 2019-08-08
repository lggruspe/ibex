import ctypes
import enum
import os
import sys
sys.path.append(os.path.join(os.path.dirname(__file__), "redblackpy"))
from redblack import containers

_rnd = ctypes.CDLL("libcrnd.so")

class _CInterval(ctypes.Structure):
    _fields_ = [
            ("start", ctypes.c_int),
            ("end", ctypes.c_int)
            ]

class _CExpr(ctypes.Structure):
    pass
_CExpr._fields_ = [
        ("type", ctypes.c_char),
        ("left", ctypes.POINTER(_CExpr)),
        ("right", ctypes.POINTER(_CExpr)),
        ("symbols", _CInterval)
        ]

class _CTransition(ctypes.Structure):
    _fields_ = [
            ("current_state", ctypes.c_int),
            ("next_state", ctypes.c_int),
            ("symbols", _CInterval)
            ]

class _CDfa(ctypes.Structure):
    _fields_ = [
            ("number_states", ctypes.c_size_t),
            ("number_transitions", ctypes.c_size_t),
            ("number_accept_states", ctypes.c_size_t),
            ("start_state", ctypes.c_int),
            ("transitions", ctypes.POINTER(_CTransition)),
            ("accept_states", ctypes.POINTER(ctypes.c_int)),
            ("error", ctypes.c_char_p)
            ]

_rnd_convert = _rnd.rnd_convert
_rnd_convert.argtypes = [ctypes.POINTER(_CExpr)]
_rnd_convert.restype = _CDfa

_rnd_dfa_destroy = _rnd.rnd_dfa_destroy
_rnd_dfa_destroy.argtypes = [ctypes.POINTER(_CDfa)]
_rnd_dfa_destroy.restype = None

_rnd_expr_symbol = _rnd.rnd_expr_symbol
_rnd_expr_symbol.argtypes = [ctypes.c_int, ctypes.c_int]
_rnd_expr_symbol.restype = ctypes.POINTER(_CExpr)

_rnd_expr_union = _rnd.rnd_expr_union
_rnd_expr_union.argtypes = [ctypes.POINTER(_CExpr), ctypes.POINTER(_CExpr)]
_rnd_expr_union.restype = ctypes.POINTER(_CExpr)

_rnd_expr_concatenation = _rnd.rnd_expr_concatenation
_rnd_expr_concatenation.argtypes = [ctypes.POINTER(_CExpr), ctypes.POINTER(_CExpr)]
_rnd_expr_concatenation.restype = ctypes.POINTER(_CExpr)

_rnd_expr_closure = _rnd.rnd_expr_closure
_rnd_expr_closure.argtypes = [ctypes.POINTER(_CExpr)]
_rnd_expr_closure.restype = ctypes.POINTER(_CExpr)

_rnd_expr_destroy = _rnd.rnd_expr_destroy
_rnd_expr_destroy.argtypes = [ctypes.POINTER(_CExpr)]
_rnd_expr_destroy.restype = None

_rnd_expr_free = _rnd.rnd_expr_free
_rnd_expr_free.argtypes = [ctypes.POINTER(_CExpr)]
_rnd_expr_free.restype = None

_rnd_get_expr_counter = _rnd.rnd_get_expr_counter
_rnd_get_expr_counter.restype = ctypes.c_int

class ExprType(enum.Enum):
    SYMBOL = 0
    UNION = 1
    CONCATENATION = 2
    CLOSURE = 3

class ExprSymbols:
    def __init__(self, start: int=0, end: int=None):
        if end is None:
            end = start
        if start > end:
            raise ValueError("start must be <= end")
        self.start = start
        self.end = end
        self._cpointer = _rnd_expr_symbol(start, end)

    def union(self, other):
        return union(self, other)

    def concatenation(self, other):
        return concatenation(self, other)

    def closure(self):
        return closure(self)

    def destroy(self):
        if self._cpointer:
            _rnd_expr_free(self._cpointer)
        self._cpointer = None

    def __repr__(self):
        if self.start == self.end:
            return str(self.start)
        return f"[{self.start}, {self.end}]"

def _get_expr_pointer(expr):
    return None if not expr else expr._cpointer

# doesn't raise exceptions to avoid memory leak
class Expr:
    def __init__(self, type_, left=None, right=None):
        self.type_ = type_
        self.left = left
        self.right = right
        self._cpointer = None

        left = _get_expr_pointer(self.left)
        if type_ is ExprType.UNION:
            self._cpointer = _rnd_expr_union(left,
                    _get_expr_pointer(self.right))
        elif type_ is ExprType.CONCATENATION:
            self._cpointer = _rnd_expr_concatenation(left,
                    _get_expr_pointer(self.right))
        elif type_ is ExprType.CLOSURE:
            self._cpointer = _rnd_expr_closure(left)

    def __repr__(self):
        if self.type_ == ExprType.UNION:
            return f"union({self.left!r}, {self.right!r})"
        elif self.type_ == ExprType.CONCATENATION:
            return f"concatenation({self.left!r}, {self.right!r})"
        elif self.type_ == ExprType.CLOSURE:
            return f"closure({self.left!r})"

    def destroy(self):
        if self._cpointer:
            _rnd_expr_free(self._cpointer)
        self._cpointer = None
        if self.left:
            self.left.destroy()
        if self.right:
            self.right.destroy()
        self.left = None
        self.right = None

    def union(self, other):
        return union(self, other)

    def concatenation(self, other):
        return concatenation(self, other)

    def closure(self):
        return closure(self)

def union(a, b) -> Expr:
    return Expr(ExprType.UNION, a, b)

def concatenation(a, b) -> Expr:
    return Expr(ExprType.CONCATENATION, a, b)

def closure(expr: Expr or ExprSymbols) -> Expr:
    return Expr(ExprType.CLOSURE, expr)

class DfaSymbols:
    def __init__(self, start: int=0, end: int=None):
        if end is None:
            end = start
        if start > end:
            raise ValueError("start must be <= end")
        self.start = start
        self.end = end

    def __repr__(self):
        if self.start == self.end:
            return str(self.start)
        return f"[{self.start}, {self.end}]"

    def __lt__(self, other):
        assert self.start <= self.end
        assert other.start <= other.end
        return self.end < other.start

    def __gt__(self, other):
        assert self.start <= self.end
        assert other.start <= other.end
        return self.start > other.end

    def __eq__(self, other):
        """Checks if the closed intervals [self.start, self.end] and
        [other.start, other.end] overlap."""
        assert self.start <= self.end
        assert other.start <= other.end
        return not (self < other) and not (self > other)

class Dfa:
    def __init__(self):
        self.start = -1
        self.accepts = set()
        self.transitions = {}

    def __repr__(self):
        return f"<rnd.Dfa start={self.start!r}, accepts={self.accepts!r}, "\
                f"transitions={self.transitions!r}>"

    def compute(self, inputs):
        """Compute if Dfa accepts string of inputs.
        continue.

        Assume that -1 is an error state with no outbound transitions.
        """
        state = self.start
        for a in inputs:
            if state not in self.transitions:
                return False
            state = self.transitions[state].get(DfaSymbols(a), -1)
        return state in self.accepts

def _cdfa_to_pydfa(_dfa: _CDfa) -> Dfa:
    dfa = Dfa()
    dfa.start = int(_dfa.start_state)
    dfa.transitions[dfa.start] = containers.Map()

    n = int(_dfa.number_accept_states)
    for i in range(n):
        accept = int(_dfa.accept_states[i])
        dfa.accepts.add(accept)
        dfa.transitions[accept] = containers.Map()

    n = (_dfa.number_transitions)
    for i in range(n):
        _trans = _dfa.transitions[i]
        q = int(_trans.current_state)
        r = int(_trans.next_state)

        a_start = int(_trans.symbols.start)
        a_end = int(_trans.symbols.end)
        a = DfaSymbols(a_start, a_end)

        if q not in dfa.transitions:
            dfa.transitions[q] = containers.Map()
        if r not in dfa.transitions:
            dfa.transitions[r] = containers.Map()
        # assumes a doesn't overlap with any other DfaSymbols
        dfa.transitions[q][a] = r
    return dfa

def convert(expr: Expr or ExprSymbols) -> Dfa:
    _dfa = _rnd_convert(expr._cpointer)
    dfa = _cdfa_to_pydfa(_dfa)
    _rnd_dfa_destroy(ctypes.byref(_dfa))
    return dfa
