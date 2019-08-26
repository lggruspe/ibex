#pragma once
#include <iostream>
#include <string>
#include <utility>

namespace match {

template <class Scanner>
std::string single(std::istream& in, Scanner& scanner, std::string lexeme="")
{
    for (int c = in.get(); c != EOF; c = in.get()) {
        lexeme += (char)c;
        if (!scanner.next(c)) {
            int steps = scanner.backtrack();
            std::cout << "steps: " << steps << std::endl;
            std::cout << "lexeme.size: " << lexeme.size() << std::endl;
            for (int i = 0; i < steps; ++i) {
                in.unget();
                if (!lexeme.empty()) {
                    in.putback((int)(lexeme.back()));
                    lexeme.pop_back();
                }
            }
            break;
        }
    }
    return lexeme;
}

}
