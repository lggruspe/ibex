import argparse
import csv
import random

test_data_generators = set()

def test_data_generator(f):
    """Decorator that automatically registers f in test_data_generators."""
    test_data_generators.add(f)
    return f

@test_data_generator
def empty():
    return ''

@test_data_generator
def number():
    digits = "0123456789"
    def integer():
        n = random.randint(1, 9)
        if n == 1:
            return random.choice(digits)

        rv = random.choice(digits[1:])
        for i in range(1, n):
            rv += random.choice(digits)
        return rv

    rv = integer()
    if random.randint(0, 1):
        rv += "."
        for _ in range(random.randint(1, 9)):
            rv += random.choice(digits)
    if random.randint(0, 1):
        rv += random.choice("eE")
        if random.randint(0, 1):
            rv += random.choice("+-")
        rv += integer()
    return rv

@test_data_generator
def character():
    escape = "\\{}".format(random.choice("nt\\"))
    char = chr(random.choice([32, 38] + [40, 91] + [93, 126]))
    return "'{}'".format(random.choice([escape, char]))

@test_data_generator
def string():
    char = chr(random.choice([32, 33] + [35, 91] + [93, 126]))
    escape = "\\{}".format(chr(random.choice([32, 126])))
    string = ""
    for i in range(random.randint(0, 32)):
        string += random.choice([char, escape])
    return f'"{string}"'

@test_data_generator
def whitespace():
    return random.choice([' ', "\\t", "\\n"])

@test_data_generator
def identifier():
    A = "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
    B = "0123456789"
    rv = random.choice(A)
    for _ in range(random.randint(0, 15)):
        rv += random.choice(A + B)
    return rv

def create_examples(f, n: int, p: float) -> {(str, bool)}:
    """Create ~n*p true examples for f and ~n*(1-p) false examples.

    The output isn't guaranteed to have n examples, because duplicate examples
    can be generated, which are not added to the output.
    """
    assert f in test_data_generators
    assert 0 <= p <= 1
    generators = test_data_generators - {f}
    examples = set()
    for i in range(n):
        g = random.sample(generators, 1)[0]
        h = f if random.random() < p else g
        example = (h(), int(h == f))
        examples.add(example)
    return examples

def output_examples(examples, filename=""):
    if filename:
        with open(filename, "w") as f:
            writer = csv.writer(f)
            writer.writerows(examples)
    else:
        for example in examples:
            print(example[0], example[1])

def get_args(description):
    parser = argparse.ArgumentParser(description=description)
    parser.add_argument("-n",
            type=int,
            default=30,
            help="number of examples for each type of test (default: 30)")
    parser.add_argument("-p",
            type=float,
            default=0.8,
            help="ratio of positive examples (default: 0.8)")
    parser.add_argument("-od",
            default=".",
            dest="outdir",
            help="output directory (default: current directory")
    return parser.parse_args()

if __name__ == "__main__":
    args = get_args("Generate random test data for test.py")
    n = args.n
    p = args.p
    outdir = args.outdir

    for f in test_data_generators:
        examples = create_examples(f, n, p)
        filename = outdir + "/" + f.__name__ + ".csv"
        output_examples(examples, filename)
