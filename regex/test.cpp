#include "utilities.h"
#include "regex.h"
#include <iostream>
#include <string>

using namespace regex;

int main()
{
    Expr eps = empty();
    Expr id = identifier();
    Expr intgr = integer();
    Expr re = real();
    
    std::cout << to_string(eps) << std::endl;
    std::cout << to_string(id) << std::endl;
    std::cout << to_string(intgr) << std::endl;
    std::cout << to_string(re) << std::endl;
}
