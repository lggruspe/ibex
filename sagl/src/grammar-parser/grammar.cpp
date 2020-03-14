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
    try {
        auto root = std::move(p.parse(callback));
        std::cout << root << std::endl;
    } catch (SyntaxError e) {
        std::cout << e.what() << std::endl;
    }
}
