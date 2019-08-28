#pragma once
#include <iostream>
#include <string>
#include <utility>

namespace match {

template <class Scanner>
std::string fsingle(std::istream& in, std::string lexeme="")
{
    Scanner scanner;
    for (int c = in.get(); c != EOF; c = in.get()) {
        lexeme += (char)c;
        if (!scanner.next(c)) {
            for (auto it = lexeme.rbegin(); it != lexeme.rend(); ++it) {
                in.putback((int)(*it));
            }
            for (int i = 0; i < scanner.backtrack_steps(); ++i) {
                lexeme.pop_back();
            }
            break;
        }
    }
    return lexeme;
}

template <class Scanner>
std::string single(std::string lexeme="")
{
    return fsingle<Scanner>(std::cin, lexeme);
}
}
