#include "thompson.h"

int main()
{
    auto a = Expr(SymbolInterval(1, 2));
    auto b = Expr(SymbolInterval(2, 3));
    auto c = concatenate(a, b);
    std::cout << c << std::endl;
}
