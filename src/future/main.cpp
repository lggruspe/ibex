#include "thompson.h"

int main()
{
    /*
    auto a = Expr(SymbolInterval(1, 3));
    std::cout << a << std::endl;

    auto b = closure(a);
    std::cout << b << std::endl;
    */

    auto a = Expr(SymbolInterval(1, 2));
    auto b = Expr(SymbolInterval(2, 3));
    auto c = concatenate(a, b);
    std::cout << c << std::endl;

    /*
    SymbolInterval a(1, 4);
    SymbolInterval b(6, 7);
    SymbolInterval c(5, 6);
    SymbolInterval d(1, 11);
    std::set<SymbolInterval> s = {a, b, c};
    for (const auto& x: s) {
        std::cout << x << " " << d << " ";
        std::cout << (x == d) << std::endl;
    }
    */
}
