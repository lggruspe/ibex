#include "../output/scanners.hpp"
#include "match.hpp"
#include <iostream>
#include <string>
#include <tuple>

int main()
{
    /*
    std::string lexeme = match::single<NumberScanner>();
    std::string token = (lexeme.empty() ? "SyntaxError" : "Number");
    std::cout << "token: " << token << std::endl;
    std::cout << "lexeme: " << lexeme << std::endl;
    */
   
    /*
    auto [token, lexeme] = match::longest<Token, Scanner, NumberScanner, IdentifierScanner>();
    std::cout << "token: " << token << std::endl;
    std::cout << "lexeme: " << lexeme << std::endl;
    */

    match::Tokenizer<SCAN_ALL> tokenizer;
    for (auto [token, lexeme] = tokenizer.tokenize(); 
            !tokenizer.done;
            std::tie(token, lexeme) = tokenizer.tokenize()) {
        std::cout << "token: " << token << std::endl;
        std::cout << "lexeme: " << lexeme << std::endl;
    }
}
