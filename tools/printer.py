import gdb

class ExprPrinter:
    def __init__(self, val):
        self.val = val

    def to_string(self):
        return "Hello!"

def lookup_type(val):
    if str(val.type) == "std::__shared_ptr<regex::_Expr, (__gnu_cxx::_Lock_policy)2>":
        return ExprPrinter(val)

gdb.pretty_printers.append(lookup_type)
