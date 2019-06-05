#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

enum Token {
    Empty,
    Other,
    Identifier,
    Ignore,
    Number
};

std::ostream& operator<<(std::ostream& out, Token token)
{
    switch(token) {
    case Other:
        return out << "Other" << std::endl;
    case Empty:
        return out << "Empty" << std::endl;
    case Identifier:
        return out << "Identifier" << std::endl;
    case Number:
        return out << "Number" << std::endl;
    default:
        return out;
    }
}

struct Scanner {
    Token token;
    Scanner(Token token) : token(token) {}
    virtual std::string operator()(std::istream&) = 0;
};

struct IdentifierScanner: public Scanner {
    using Scanner::Scanner;
    std::string operator()(std::istream& in)
    {
        char c;
        std::vector<char> checkpoint;
        std::string lexeme;
        goto s0;
    s0:
        in.get(c);
        lexeme += c;
        checkpoint.push_back(c);
        if ((65 <= c && c <= 90) || (95 <= c && c <= 95) || (97 <= c && c <= 122)) {
            goto s1;
        }
        goto se;
    s1:
        in.get(c);
        lexeme += c;
        checkpoint.clear();
        checkpoint.push_back(c);
        if ((48 <= c && c <= 57) || (65 <= c && c <= 90) || (95 <= c && c <= 95) || (97 <= c && c <= 122)) {
            goto s1;
        }
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

struct IgnoreScanner: public Scanner {
    using Scanner::Scanner;
    std::string operator()(std::istream& in)
    {
        char c;
        std::vector<char> checkpoint;
        std::string lexeme;
        goto s0;
    s0:
        in.get(c);
        lexeme += c;
        checkpoint.push_back(c);
        if ((9 <= c && c <= 9) || (10 <= c && c <= 10) || (32 <= c && c <= 32)) {
            goto s1;
        }
        goto se;
    s1:
        in.get(c);
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

struct NumberScanner: public Scanner {
    using Scanner::Scanner;
    std::string operator()(std::istream& in)
    {
        char c;
        std::vector<char> checkpoint;
        std::string lexeme;
        goto s0;
    s0:
        in.get(c);
        lexeme += c;
        checkpoint.push_back(c);
        if ((49 <= c && c <= 57)) {
            goto s1;
        }
        if ((48 <= c && c <= 48)) {
            goto s5;
        }
        goto se;
    s1:
        in.get(c);
        lexeme += c;
        checkpoint.clear();
        checkpoint.push_back(c);
        if ((48 <= c && c <= 48) || (49 <= c && c <= 57)) {
            goto s1;
        }
        if ((69 <= c && c <= 69) || (101 <= c && c <= 101)) {
            goto s2;
        }
        if ((46 <= c && c <= 46)) {
            goto s7;
        }
        goto se;
    s2:
        in.get(c);
        lexeme += c;
        checkpoint.push_back(c);
        if ((43 <= c && c <= 43) || (45 <= c && c <= 45)) {
            goto s4;
        }
        if ((48 <= c && c <= 48)) {
            goto s6;
        }
        if ((49 <= c && c <= 57)) {
            goto s8;
        }
        goto se;
    s3:
        in.get(c);
        lexeme += c;
        checkpoint.clear();
        checkpoint.push_back(c);
        if ((69 <= c && c <= 69) || (101 <= c && c <= 101)) {
            goto s2;
        }
        if ((48 <= c && c <= 48) || (49 <= c && c <= 57)) {
            goto s3;
        }
        goto se;
    s4:
        in.get(c);
        lexeme += c;
        checkpoint.push_back(c);
        if ((48 <= c && c <= 48)) {
            goto s6;
        }
        if ((49 <= c && c <= 57)) {
            goto s8;
        }
        goto se;
    s5:
        in.get(c);
        lexeme += c;
        checkpoint.clear();
        checkpoint.push_back(c);
        if ((69 <= c && c <= 69) || (101 <= c && c <= 101)) {
            goto s2;
        }
        if ((46 <= c && c <= 46)) {
            goto s7;
        }
        goto se;
    s6:
        in.get(c);
        lexeme += c;
        checkpoint.clear();
        checkpoint.push_back(c);
        goto se;
    s7:
        in.get(c);
        lexeme += c;
        checkpoint.push_back(c);
        if ((48 <= c && c <= 48) || (49 <= c && c <= 57)) {
            goto s3;
        }
        goto se;
    s8:
        in.get(c);
        lexeme += c;
        checkpoint.clear();
        checkpoint.push_back(c);
        if ((48 <= c && c <= 48) || (49 <= c && c <= 57)) {
            goto s8;
        }
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
        scanners.push_back(std::make_shared<IdentifierScanner>(Identifier));
        scanners.push_back(std::make_shared<IgnoreScanner>(Ignore));
        scanners.push_back(std::make_shared<NumberScanner>(Number));
    }

    std::pair<Token, std::string> operator()()
    {
        for (auto scanner: scanners) {
            std::string lexeme = (*scanner)(*in);
            if (!lexeme.empty()) {
                if (scanner->token != Ignore) {
                    return std::pair<Token, std::string>(scanner->token, lexeme);
                }
                return (*this)();
            }
        }
        int c = in->get();
        if (c == std::char_traits<char>::eof()) {
            in->clear();
            return std::pair<Token, std::string>(Empty, "");
        }
        return std::pair<Token, std::string>(Other, std::string(1, (char)c));
    }

    void scan()
    {
        for (;;) {
            try {
                auto [token, lexeme] = (*this)();
                std::cout << "token: " << token << std::endl;
                std::cout << "lexeme: " << lexeme << std::endl;
                std::cout << std::endl;
            } catch (const char* e) {
                in->ignore(1, '\n');
            }
        }
    }
};

