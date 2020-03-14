#include "sagl.hpp"
#include "parser.hpp"
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
}
