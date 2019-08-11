import jinja2
import argparse
import rnd

scanners = []

def symbols(a: str, b: str=None) -> rnd.ExprSymbols:
    if b is None:
        b = a
    a, b = ord(a), ord(b)
    return rnd.ExprSymbols(a, b)

def isymbols(a: int, b: int=None) -> rnd.ExprSymbols:
    if b is None:
        b = a
    return rnd.ExprSymbols(a, b)

def optional(expr: rnd.ExprSymbols or rnd.Expr):
    return expr.union(symbols('\0'))

def token(name):
    def decorator(f):
        def wrapper():
            expr = f()
            dfa = rnd.convert(expr)
            expr.destroy()
            dfa.token = name
            return dfa
        scanners.append(wrapper())
        return wrapper
    return decorator

def get_args():
    description = "Generate scanner using jinja2 templates."
    parser = argparse.ArgumentParser(description=description)
    parser.add_argument("-i",
            default="",
            dest="basetemp",
            help="filename of base template")
    parser.add_argument("-d",
            default="",
            dest="tempdir",
            help="path to templates directory")
    return parser.parse_args()

def generate_code(basetemp, tempdir):
    loader = jinja2.FileSystemLoader(tempdir)
    env = jinja2.Environment(loader=loader, line_statement_prefix="##")
    template = env.get_template(basetemp)
    return template.render(scanners=scanners)

def generate(basetemp="", tempdir=""):
    args = get_args()
    if args.basetemp:
        basetemp = args.basetemp
    if args.tempdir:
        tempdir = args.tempdir

    try:
        output = generate_code(basetemp, tempdir)
        print(output)
    except jinja2.exceptions.TemplateNotFound:
        print("scangen: Template not found:", basetemp)

if __name__ == "__main__":
    generate()
