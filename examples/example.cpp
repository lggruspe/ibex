#include "parser.hpp"
#include "parsetree.hpp"
#include "scanner.hpp"
#include <iostream>

using namespace parser;

int main()
{
    ParseTreeCallback cb;
    bool ok = parse(std::cin, &cb);
    auto root = std::move(cb.ast(ok));
    std::cout << root << std::endl;
}
