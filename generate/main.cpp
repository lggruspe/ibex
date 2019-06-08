#include "generator.h"
#include "utilities.h"
#include <iostream>

using namespace regex2;

int main()
{
    gen2::generate(std::cout, {
            {"Number", number()},
            {"Identifier", identifier()},
            {"Character", character()},
            {"String", string()},
            {"Ignore", whitespace()}    // alternate regex to be ignored
    });
}
