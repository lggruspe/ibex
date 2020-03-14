#include "grammar-parser/grammar_parser.h"
#include <iostream>

int main()
{
    auto s = parse_grammar(R"(
        S -> A.
        A -> a A b.
        A -> .
    )");
    std::cout << s << std::endl;
}
