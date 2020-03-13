#include "sagl.hpp"
#include "parser.hpp"
#include "parsetree.hpp"
#include "scanner.hpp"
#include <iostream>

int main()
{
    Grammar g({
        {"S", {"A"}},
        {"A", {"a", "A", "b"}},
        {"A", {}},
    });
    Parser p(g);
    ParseTreeCallback<decltype(g)::Symbol> callback;
    try {
        auto root = std::move(p.parse(callback));
        std::cout << root << std::endl;
    } catch (SyntaxError e) {
        std::cout << e.what() << std::endl;
    }
}
