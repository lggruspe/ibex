"""A scanner generator that uses Jinja2 templates."""
import os.path as op
import jinja2
import scangen.internals as regexp

def from_class(cls):
    """Does not include token names that start with _."""
    keys = [k for k in cls.__dict__.keys() if k[0] != '_']
    return {k:cls.__dict__[k] for k in keys}

def convert(tokens):
    scanner = []
    for k, v in tokens.items():
        dfa = regexp.DFA(v)
        dfa.token = k
        scanner.append(dfa)
    return scanner

def render(tokens, entrypoint, directory=None, config=None):
    paths = [op.abspath(op.join(op.dirname(__file__), "templates"))]
    if directory:
        paths = [directory, *paths]
    loader = jinja2.FileSystemLoader(paths)
    env = jinja2.Environment(loader=loader)
    template = env.get_template(entrypoint)
    return template.render(scanner=convert(tokens), config=config)

name = "scangen"
