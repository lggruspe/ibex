#include "../output/scanners.hpp"
#include "match.hpp"
#include <iostream>
#include <string>

int main()
{
    EmptyScanner empty;
    IdentifierScanner identifier;
    WhitespaceScanner whitespace;
    NumberScanner number;
    CharacterScanner character;
    StringScanner string;
    
    std::string lexeme = match::single(std::cin, number);
    std::string token = (lexeme.empty() ? "SyntaxError" : "Number");
    std::cout << "token: " << token << std::endl;
    std::cout << "lexeme: " << lexeme << std::endl;
}
