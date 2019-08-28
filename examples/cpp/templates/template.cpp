#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#define SCAN_ALL \
    Token,\
    Scanner,\
## for scanner in scanners
## if loop.index == loop.length
    {{ scanner.token|title }}Scanner

## else
    {{ scanner.token|title }}Scanner,\
## endif
## endfor

enum class Token {
    empty,
## for scanner in scanners
    {{ scanner.token }},
## endfor
};

std::ostream& operator<<(std::ostream& out, Token token)
{
    switch (token) {
    case Token::empty:
        return out << "empty";
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
    bool accepts;

    Scanner(Token token) 
        : token(token)
        , checkpoint({0})
        , accepts(false) {}

    virtual ~Scanner() {}
    virtual bool next(int) = 0;

    int state() const
    {
        return checkpoint.empty() ? -1 : checkpoint.back();
    }

    int change_state(int next_state, bool checkpoint=false)
    {
        if (checkpoint) {
            this->checkpoint.clear();
            accepts = true;
        }
        this->checkpoint.push_back(next_state);
        return next_state != -1;
    }

    int backtrack_steps() const
    {
        return checkpoint.size() - 1;
    }
};

std::ostream& operator<<(std::ostream& out, const Scanner& scanner)
{
    return out << "<Scanner " << scanner.token << " state:" << scanner.state()
        << " checkpoint:[";
    for (const auto& state: scanner.checkpoint) {
        out << " " << state;
    }
    return out << " ]>";
}

## for scanner in scanners
{% include "scanner.cpp" %}

## endfor
