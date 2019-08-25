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
    Scanner(Token token) : token(token) {}
    virtual ~Scanner() {}
    virtual std::string operator()(std::istream&) = 0;
};

## for scanner in scanners
{% include "scanner.cpp" %}

## endfor

struct ScannerCollection {
    std::vector<std::shared_ptr<Scanner>> scanners;
    std::istream* in;

    ScannerCollection(std::istream& in=std::cin) : in(&in)
    {
        //scanners.push_back(std::make_shared<_IgnoreScanner>(Token::_IgnoreScanner));
## for scanner in scanners
        scanners.push_back(std::make_shared<{{ scanner.token|title }}Scanner >(Token::{{ scanner.token }}));
## endfor
    }

    std::pair<Token, std::string> operator()()
    {
        for (auto scanner: scanners) {
            std::string lexeme = (*scanner)(*in);
            if (!lexeme.empty()) {
                if (scanner->token != Token::_ignore) {
                    return std::pair<Token, std::string>(scanner->token, lexeme);
                }
                return (*this)();
            }
        }
        int c = in->get();
        if (c == std::char_traits<char>::eof()) {
            in->clear();
            in->putback(c);
            in->ignore();
            return std::pair<Token, std::string>(Token::_empty, "");
        }
        return std::pair<Token, std::string>(Token::_other, std::string(1, (char)c));
    }

    void scan()
    {
        for (;;) {
            auto [token, lexeme] = (*this)();
            std::cout << "token: " << token << std::endl;
            std::cout << "lexeme: " << lexeme << std::endl;
            std::cout << std::endl;
        }
    }
};
