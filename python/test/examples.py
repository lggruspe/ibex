import argparse
import csv
import random

INSTANCE_GENERATORS = set()

def instances(fn):
    """Register fn as a random instance generator."""
    INSTANCE_GENERATORS.add(fn)
    return fn

class ExampleGenerator:
    def __init__(self, positive, negative):
        assert positive
        assert negative
        self.positive = set(positive)
        self.negative = set(negative)
        assert self.positive.issubset(INSTANCE_GENERATORS)
        assert self.negative.issubset(INSTANCE_GENERATORS)

    def positive_example(self):
        return random.sample(self.positive, 1)[0]()

    def negative_example(self):
        return random.sample(self.negative, 1)[0]()

    def example(self, p=0.5):
        if random.random() < p:
            return (self.positive_example(), 1)
        return (self.negative_example(), 0)

def create_examples(f, n: int, p: float) -> {(str, bool)}:
    """Create ~n*p true examples for f and ~n*(1-p) false examples.

    The output isn't guaranteed to have n examples, because duplicate examples
    can be generated, which are not added to the output.
    """
    example_generator = ExampleGenerator([f], INSTANCE_GENERATORS - {f})
    return {example_generator.example(p) for _ in range(n)}

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
                help="number of examples for each instance type (default: 30)")
        parser.add_argument("-p",
                type=float,
                default=0.8,
                help="ratio of positive examples (default: 0.8)")
        parser.add_argument("-o",
                default=".",
                dest="outdir",
                help="output directory (default: current directory)")
        return parser.parse_args()

    args = get_args("Generate classifier examples")
    n = args.n
    p = args.p
    outdir = args.outdir

    for f in INSTANCE_GENERATORS:
        examples = create_examples(f, n, p)
        filename = outdir + "/" + f.__name__ + ".csv"
        output_examples(examples, filename)

if __name__ == "__main__":
    main()
