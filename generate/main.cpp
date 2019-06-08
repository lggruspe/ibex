#include "generator.h"
#include "utilities.h"
#include <iostream>

using namespace regex2;

int main()
{
    gen2::generate(std::cout, {
            {"number", number()},
            {"identifier", identifier()},
            {"character", character()},
            {"string", string()},
            {"_ignore", whitespace()}    // alternate regex to be ignored
    });
}
