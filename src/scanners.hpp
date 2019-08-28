#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#define SCAN_ALL \
    Token,\
    Scanner,\
    AScanner

enum class Token {
    empty = 0,
    a,
};

std::ostream& operator<<(std::ostream& out, Token token)
{
    switch (token) {
    case Token::empty:
        return out << "empty";
    case Token::a:
        return out << "a";
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

struct AScanner: public Scanner {
    AScanner() : Scanner(Token::a) {}
    bool next(int c)
    {
        switch (state()) {
        case 0:
            if (c == 97)
                return change_state(1, true);
            return change_state(-1);
        case 1:
            return change_state(-1);
        default:
            return change_state(-1);
        }
    }
};


