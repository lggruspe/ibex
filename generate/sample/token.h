#include <iostream>

enum Token { Whitespace, Number, Identifier, Other, Empty, Ignore };

std::ostream& operator<<(std::ostream& out, Token token)
{
    switch (token) {
    case Whitespace:
        return out << "whitespace" << std::endl;
    case Number:
        return out << "number" << std::endl;
    case Identifier:
        return out << "identifier" << std::endl;
    case Other:
        return out << "other" << std::endl;
    default:
        return out;
    }
}


