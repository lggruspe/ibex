import ctypes
import enum
import os
import sys
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), os.pardir, "redblackpy")))
import internals
from internals import crnd
from redblack import containers

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
        self._cpointer = crnd.rnd_expr_symbol(start, end)

    def union(self, other):
        return union(self, other)

    def concatenation(self, other):
        return concatenation(self, other)

    def closure(self):
        return closure(self)

    def destroy(self):
        if self._cpointer:
            crnd.rnd_expr_free(self._cpointer)
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
            self._cpointer = crnd.rnd_expr_union(left,
                    _get_expr_pointer(self.right))
        elif type_ is ExprType.CONCATENATION:
            self._cpointer = crnd.rnd_expr_concatenation(left,
                    _get_expr_pointer(self.right))
        elif type_ is ExprType.CLOSURE:
            self._cpointer = crnd.rnd_expr_closure(left)

    def __repr__(self):
        if self.type_ == ExprType.UNION:
            return f"union({self.left!r}, {self.right!r})"
        elif self.type_ == ExprType.CONCATENATION:
            return f"concatenation({self.left!r}, {self.right!r})"
        elif self.type_ == ExprType.CLOSURE:
            return f"closure({self.left!r})"

    def destroy(self):
        if self._cpointer:
            crnd.rnd_expr_free(self._cpointer)
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

def _cdfa_to_pydfa(_dfa: internals.CDfa) -> Dfa:
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
    _dfa = crnd.rnd_convert(expr._cpointer)
    dfa = _cdfa_to_pydfa(_dfa)
    crnd.rnd_dfa_destroy(ctypes.byref(_dfa))
    return dfa
