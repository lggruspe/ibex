#include "sagl.hpp"
#include "grammar_parser.h"
#include "parser.hpp"
#include "parsetree.hpp"
#include "scanner.hpp"
#include <iostream>

using namespace scanner;

int main()
{
    Grammar g({
        {"Grammar", {"Rules"}},
        {"Rules",   {"Rules", "Rule"}},
        {"Rules",   {}},
        {"Rule",    {"Lhs", "arrow", "Rhs", "dot"}},
        {"Lhs",     {"identifier"}},
        {"Rhs",     {"Rhs", "identifier"}},
        {"Rhs",     {}},
    });
    Parser p(g);
    ParseTreeCallback callback;

    std::vector<std::pair<std::string, std::vector<std::string>>> rules;
    try {
        rules = p.parse(callback);
    } catch (SyntaxError e) {
        std::cout << e.what() << std::endl;
    }

    Grammar result(rules);
    Table t(result);
    auto s = t.jsonify();
    std::cout << s << std::endl;
    auto u = parse_grammar(R"(
        Grammar -> Rules.
        Rules   -> Rules Rule.
        Rules   -> .
        Rule    -> Lhs arrow Rhs dot.
        Lhs     -> identifier.
        Rhs     -> Rhs identifier.
        Rhs     -> .
    )");

    std::cout << "---" << std::endl;
    std::cout << u << std::endl;
}
