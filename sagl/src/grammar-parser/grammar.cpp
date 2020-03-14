#include "sagl.hpp"
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

    std::cout << rules.size() << std::endl;
    Grammar result(rules);
}
