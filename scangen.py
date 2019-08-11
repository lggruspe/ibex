import argparse
import jinja2
import rnd
import sys

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

def get_args(args):
    description = "Generate scanner using jinja2 templates."
    parser = argparse.ArgumentParser(description=description)
    parser.add_argument("entrypoint",
            help="filename of template entrypoint")
    parser.add_argument("-d",
            default="",
            dest="directory",
            help="path to templates directory")
    return parser.parse_args(args)

def generate_code(entrypoint, directory):
    loader = jinja2.FileSystemLoader(directory)
    env = jinja2.Environment(loader=loader, line_statement_prefix="##")
    template = env.get_template(entrypoint)
    return template.render(scanners=scanners)

def generate(entrypoint="", directory=""):
    args = sys.argv[1:]
    if entrypoint:
        args.append(entrypoint)
    if directory:
        args.extend(["-d", directory])
    args = get_args(args)
    if args.entrypoint:
        entrypoint = args.entrypoint
    if args.directory:
        directory = args.directory

    try:
        output = generate_code(entrypoint, directory)
        print(output)
    except jinja2.exceptions.TemplateNotFound:
        print("scangen: Template not found:", entrypoint)

if __name__ == "__main__":
    generate()
