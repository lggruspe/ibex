#include "foobar.h"
#include <iostream>

int main()
{
    floatScanner scan(std::cin);
    auto res = scan();
    std::cout << res.first << ' ' << res.second << std::endl;
}
