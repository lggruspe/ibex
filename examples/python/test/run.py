import argparse
import os.path
import sys
import match

sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), "..")))

import output.scanners as scanners

def test_longest_match():
    token, lexeme = match.longest(*(scanners.SCANNERS.values()))
    print(token)
    print(lexeme)

def test_single_match(scanner):
    lexeme = match.single(scanner)
    print(lexeme)

def test_tokenizer():
    tokenize = match.tokenizer(*(scanners.SCANNERS.values()))
    for token, lexeme in tokenize:
        print(token, lexeme)

def main():
    def get_args():
        description = "Run python scangen example."
        parser = argparse.ArgumentParser(description=description)
        subparsers = parser.add_subparsers(
            dest="subcommand",
            help="sub-command help")
        parser_single = subparsers.add_parser(
            "single",
            help="match input to a single scanner")
        parser_single.add_argument(
            "-s",
            dest="scanner",
            help="name of scanner ({})".format('|'.join(scanners.SCANNERS.keys())))
        parser_longest = subparsers.add_parser(
            "longest",
            help="find longest match")
        parser_tokenizer = subparsers.add_parser(
            "tokenizer",
            help="tokenize input")
        return parser.parse_args()

    args = get_args()
    if args.subcommand == "single":
        test_single_match(scanners.SCANNERS[args.scanner])
    elif args.subcommand == "longest":
        test_longest_match()
    elif args.subcommand == "tokenizer":
        test_tokenizer()

if __name__ == "__main__":
    try:
        main()
    except KeyError:
        print("Invalid argument")
    except IndexError:
        print("Missing arguments")
