// weird behavior when scanning for numbers (1+1 gets recognized)
#pragma once
#include <iostream>
#include <string>
#include <utility>
#include <vector>

enum Token { Whitespace, Integer, Number, Identifier, Ignore, Empty, Other };

std::ostream& operator<<(std::ostream& out, Token token)
{
    switch (token) {
    case Whitespace:
        return out << "whitespace";
    case Integer:
        return out << "integer";
    case Number:
        return out << "number";
    case Identifier:
        return out << "identifier";
    case Other:
        return out << "other";
    default:
        return out;
    }
}

class Scanner {
public:
    Token token;
    Scanner(Token token) : token(token) {}
    virtual std::string operator()(std::istream&) = 0;
};

class whitespaceScanner: public Scanner {
public:
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
        if ('\n' <= c && c <= '\n' || ' ' <= c && c <= ' ' || '\t' <= c && c <= '\t') {
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

class integerScanner: public Scanner {
public:
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
        if ('1' <= c && c <= '9') {
            goto s1;
        }
        if ('0' <= c && c <= '0') {
            goto s2;
        }
        goto se;

    s1:
        in.get(c);
        lexeme += c;
        checkpoint.clear();
        checkpoint.push_back(c);
        if ('1' <= c && c <= '9' || '0' <= c && c <= '0') {
            goto s1;
        }
        goto se;

    s2:
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

class floatScanner: public Scanner {
public:
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
        if ('0' <= c && c <= '0') {
            goto s1;
        }
        if ('1' <= c && c <= '9') {
            goto s5;
        }
        goto se;

    s1:
        in.get(c);
        lexeme += c;
        checkpoint.clear();
        checkpoint.push_back(c);
        if ('.' <= c && c <= '.') {
            goto s7;
        }
        if ('e' <= c && c <= 'e' || 'E' <= c && c <= 'E') {
            goto s8;
        }
        goto se;

    s2:
        in.get(c);
        lexeme += c;
        checkpoint.clear();
        checkpoint.push_back(c);
        if ('1' <= c && c <= '9' || '0' <= c && c <= '0') {
            goto s2;
        }
        if ('e' <= c && c <= 'e' || 'E' <= c && c <= 'E') {
            goto s8;
        }
        goto se;

    s3:
        in.get(c);
        lexeme += c;
        checkpoint.clear();
        checkpoint.push_back(c);
        goto se;

    s4:
        in.get(c);
        lexeme += c;
        checkpoint.push_back(c);
        if ('0' <= c && c <= '0') {
            goto s3;
        }
        if ('1' <= c && c <= '9') {
            goto s6;
        }
        goto se;

    s5:
        in.get(c);
        lexeme += c;
        checkpoint.clear();
        checkpoint.push_back(c);
        if ('1' <= c && c <= '9' || '0' <= c && c <= '0') {
            goto s5;
        }
        if ('.' <= c <= '.') {
            goto s7;
        }
        if ('e' <= c && c <= 'e' || 'E' <= c && c <= 'E') {
            goto s8;
        }
        goto se;

    s6:
        in.get(c);
        lexeme += c;
        checkpoint.clear();
        checkpoint.push_back(c);
        if ('1' <= c && c <= '9' || '0' <= c && c <= '0') {
            goto s6;
        }
        goto se;

    s7:
        in.get(c);
        lexeme += c;
        checkpoint.push_back(c);
        if ('1' <= c && c <= '9' || '0' <= c && c <= '0') {
            goto s2;
        }
        goto se;

    s8:
        in.get(c);
        lexeme += c;
        checkpoint.push_back(c);
        if ('0' <= c && c <= '0') {
            goto s3;
        }
        if ('+' <= c && c <= '+' || '-' <= c && c <= '-') {
            goto s4;
        }
        if ('1' <= c && c <= '9' || '0' <= c && c <= '0') {
            goto s6;
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

class identifierScanner: public Scanner {
public:
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
        if ('_' <= c && c <= '_' || 'a' <= c && c <= 'z' || 'A' <= c && c <= 'Z') {
            goto s1;
        }
        goto se;

    s1:
        in.get(c);
        lexeme += c;
        checkpoint.clear();
        checkpoint.push_back(c);
        if ('0' <= c && c <= '9' || '_' <= c && c <= '_' || 'a' <= c && c <= 'z' || 'A' <= c && c <= 'Z') {
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

class ScannerCollection {
    std::vector<Scanner*> scanners;
    std::istream* in;

public:
    ScannerCollection(std::istream& in=std::cin) : in(&in) {};
    void add_scanner(Scanner& scanner)
    {
        scanners.push_back(&scanner);
    }

    std::pair<Token, std::string> operator()()
    {
        for (auto& p: scanners) {
            Scanner& scanner = *p;
            std::string lexeme = scanner(*in);
            if (!lexeme.empty()) {
                if (scanner.token != Ignore) {
                    return std::pair<Token, std::string>(scanner.token, lexeme);
                }
                return (*this)();
            }
        }
        if (in->eof()) {
            return std::pair<Token, std::string>(Empty, "");
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

