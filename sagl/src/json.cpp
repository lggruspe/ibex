#include "sagl/tabulate.hpp"
#include <iostream>

int main()
{
    Grammar g({
        {"S", {"A"}},
        {"A", {"a", "A", "b"}},
        {"A", {}},
    });
    Table t(g);
    auto s = t.jsonify();
    std::cout << s << std::endl;
}
