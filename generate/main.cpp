#include "generator.h"
#include "utilities.h"
#include <iostream>

using namespace regex2;

int main()
{
    gen2::generate(std::cout, {
            {"Number", number()},
            {"Identifier", identifier()},
            {"Whitespace", whitespace()}
    });
}
