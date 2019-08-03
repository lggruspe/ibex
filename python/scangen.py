import jinja2
import argparse

class Symbol:
    def __init__(self, start=0, end=0):
        self.start = start
        self.end = end

class Scanner:
    def __init__(self):
        self.token = "token"
        self.start = 0
        self.transitions = {
                0: {
                    Symbol(0, 3): 1,
                    Symbol(4, 5): 2,
                    Symbol(6, 11): 3,
                },
                1: {
                    Symbol(): 1
                },
                3: {
                    Symbol(): 2
                }
        }
        self.accepts = { 0 }

def get_args(description):
    parser = argparse.ArgumentParser(description=description)
    parser.add_argument("template",
            help="filename of base template")
    parser.add_argument("-d",
            default=".",
            dest="tempdir",
            help="path to templates directory")
    parser.add_argument("-o",
            default="",
            dest="outfile",
            help="output file (default: stdout)")
    return parser.parse_args()

def generate_code(args, scanners):
    loader = jinja2.FileSystemLoader(args.tempdir)
    env = jinja2.Environment(loader=loader, line_statement_prefix="##")
    template = env.get_template(args.template)
    return template.render(scanners=scanners)

if __name__ == "__main__":
    args = get_args(description="Generate scanner from jinja2 templates.")
    scanner1 = Scanner()
    scanner2 = Scanner()
    scanner1.token = "scanner1"
    scanner2.token = "scanner2"
    scanner2.accepts = {1, 2, 3}
    scanners = [
        scanner1,
        scanner2
    ]

    output = None
    try:
        output = generate_code(args, scanners)
    except jinja2.exceptions.TemplateNotFound:
        print("scangen: Template not found:", args.template)

    if output:
        if not args.outfile:
            print(output)
        else:
            with open(args.outfile, "w") as f:
                f.write(output)
