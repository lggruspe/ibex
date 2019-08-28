#include "../output/scanners.hpp"
#include "match.hpp"
#include <iostream>
#include <string>

int main()
{
    /*
    std::string lexeme = match::single<NumberScanner>();
    std::string token = (lexeme.empty() ? "SyntaxError" : "Number");
    std::cout << "token: " << token << std::endl;
    std::cout << "lexeme: " << lexeme << std::endl;
    */
    
    auto [token, lexeme] = match::longest<Token, Scanner, NumberScanner, IdentifierScanner>();
    std::cout << "token: " << token << std::endl;
    std::cout << "lexeme: " << lexeme << std::endl;
}
