import argparse
import csv
import random

LEXEME_GENERATORS = set()

def randomizer(fn):
    """Register fn as a random lexeme generator."""
    LEXEME_GENERATORS.add(fn)
    return fn

def create_examples(f, n: int, p: float) -> {(str, bool)}:
    """Create ~n*p true examples for f and ~n*(1-p) false examples.

    The output isn't guaranteed to have n examples, because duplicate examples
    can be generated, which are not added to the output.
    """
    assert f in LEXEME_GENERATORS
    assert 0 <= p <= 1
    generators = LEXEME_GENERATORS - {f}
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

def main():
    def get_args(description):
        parser = argparse.ArgumentParser(description=description)
        parser.add_argument("-n",
                type=int,
                default=30,
                help="number of examples for each lexeme type (default: 30)")
        parser.add_argument("-p",
                type=float,
                default=0.8,
                help="ratio of positive examples (default: 0.8)")
        parser.add_argument("-o",
                default=".",
                dest="outdir",
                help="output directory (default: current directory)")
        return parser.parse_args()

    args = get_args("Generate random lexemes")
    n = args.n
    p = args.p
    outdir = args.outdir

    for f in LEXEME_GENERATORS:
        examples = create_examples(f, n, p)
        filename = outdir + "/" + f.__name__ + ".csv"
        output_examples(examples, filename)

if __name__ == "__main__":
    main()
