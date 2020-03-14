#include "sagl.hpp"
#include <iostream>

int main()
{
    Grammar g({
        {"Grammar", {"Rules"}},
        {"Rules",   {"Rules", "Rule"}},
        {"Rules",   {}},
        {"Rule",    {"Lhs", "arrow", "Rhs"}},
        {"Lhs",     {"identifier"}},
        {"Rhs",     {"Rhs", "identifier"}},
        {"Rhs",     {}},
    });

    Table t(g);
    auto s = t.jsonify();
    std::cout << s << std::endl;
}
