#include "parser.h"
#include "parsetree.hpp"
#include "scanner.h"
#include "exampleparser.hpp"
#include <iostream>
#include <string>

using namespace parser;

std::string getlines()
{
    std::string s, t;
    while (std::getline(std::cin, s)) {
        t += s;
    }
    return t;
}

int main()
{
    std::string s = getlines();
    auto root = ExampleParser().parse(s.c_str())->ast();
    std::cout << root << std::endl;
}
