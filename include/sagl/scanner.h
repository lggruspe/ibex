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
    a
};

std::ostream& operator<<(std::ostream& out, Token token)
{
    switch(token) {
    case Token::_other:
        return out << "_other";
    case Token::_empty:
        return out << "_empty";
    case Token::a:
        return out << "a";
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

struct _ignoreScanner: public Scanner {
    using Scanner::Scanner;
    std::string operator()(std::istream& in)
    {
        int c;
        int eof = std::char_traits<char>::eof();
        std::vector<char> checkpoint;
        std::string lexeme;
        goto s0;
    s0:
        c = in.get();
        if (c == eof) {
            goto se;
        }
        lexeme += c;
        checkpoint.push_back(c);
        if ((9 <= c && c <= 9) || (10 <= c && c <= 10) || (32 <= c && c <= 32)) {
            goto s1;
        }
        goto se;
    s1:
        c = in.get();
        if (c == eof) {
            goto se;
        }
        lexeme += c;
        checkpoint.clear();
        checkpoint.push_back(c);
        goto se;
    se:
        while (!checkpoint.empty()) {
            c = checkpoint.back();
            checkpoint.pop_back();
            in.putback(c);
            lexeme.pop_back();
        }
        return lexeme;
    }
};

struct aScanner: public Scanner {
    using Scanner::Scanner;
    std::string operator()(std::istream& in)
    {
        int c;
        int eof = std::char_traits<char>::eof();
        std::vector<char> checkpoint;
        std::string lexeme;
        goto s0;
    s0:
        c = in.get();
        if (c == eof) {
            goto se;
        }
        lexeme += c;
        checkpoint.push_back(c);
        if ((97 <= c && c <= 97)) {
            goto s1;
        }
        goto se;
    s1:
        c = in.get();
        if (c == eof) {
            goto se;
        }
        lexeme += c;
        checkpoint.clear();
        checkpoint.push_back(c);
        goto se;
    se:
        while (!checkpoint.empty()) {
            c = checkpoint.back();
            checkpoint.pop_back();
            in.putback(c);
            lexeme.pop_back();
        }
        return lexeme;
    }
};

struct ScannerCollection {
    std::vector<std::shared_ptr<Scanner>> scanners;
    std::istream* in;

    ScannerCollection(std::istream& in=std::cin) : in(&in) 
    {
        scanners.push_back(std::make_shared<_ignoreScanner>(Token::_ignore));
        scanners.push_back(std::make_shared<aScanner>(Token::a));
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

