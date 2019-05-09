#include "regex.h"
#include "utilities.h"
#include <iostream>

using namespace regex;

int main()
{
    Re eps = empty();
    Re ident = identifier();
    Re intgr = integer();
    Re re = real();

    std::cout << tostring(eps) << std::endl;
    std::cout << tostring(ident) << std::endl;
    std::cout << tostring(intgr) << std::endl;
    std::cout << tostring(re) << std::endl;

    Re a = symbol("a");
    Re b = alternate(a, eps);
    std::cout << tostring(a) << std::endl;
    std::cout << tostring(b) << std::endl;
}
