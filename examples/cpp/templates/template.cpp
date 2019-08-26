#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

enum class Token {
    _empty,
    _other,
    _ignore,
## for scanner in scanners
    {{ scanner.token }},
## endfor
};

std::ostream& operator<<(std::ostream& out, Token token)
{
    switch (token) {
    case Token::_other:
        return out << "_other";
    case Token::_empty:
        return out << "_empty";
## for scanner in scanners
    case Token::{{ scanner.token }}:
        return out << "{{ scanner.token }}";
## endfor
    default:
        return out;
    }
}

struct Scanner {
    Token token;
    std::vector<int> checkpoint;
    int accept;

    Scanner(Token token) : token(token), checkpoint({0}) {}
    virtual ~Scanner() {}
    virtual bool next(int) = 0;

    int state() const
    {
        return checkpoint.back();
    }

    int backtrack(bool clear=false)
    {
        int steps = checkpoint.size();
        if (clear) {
            checkpoint.clear();
            checkpoint.push_back(0);
        }
        return steps;
    }
};

## for scanner in scanners
{% include "scanner.cpp" %}

## endfor
