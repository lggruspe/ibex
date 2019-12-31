#pragma once
#include <iostream>

enum class Variable { START, EXPR, TERM, FACTOR, VALUE, ERROR };

std::ostream& operator<<(std::ostream& out, const Variable& v)
{
    switch (v) {
    case Variable::START:
        return out << "start";
    case Variable::EXPR:
        return out << "expr";
    case Variable::TERM:
        return out << "term";
    case Variable::FACTOR:
        return out << "factor";
    case Variable::VALUE:
        return out << "value";
    default:
        throw 0;
    }
}
