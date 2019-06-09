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
    character,
    identifier,
    number,
    string
};

std::ostream& operator<<(std::ostream& out, Token token)
{
    switch(token) {
    case Token::_other:
        return out << "_other";
    case Token::_empty:
        return out << "_empty";
    case Token::character:
        return out << "character";
    case Token::identifier:
        return out << "identifier";
    case Token::number:
        return out << "number";
    case Token::string:
        return out << "string";
    default:
        return out;
    }
}

struct Scanner {
    Token token;
    Scanner(Token token) : token(token) {}
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

struct characterScanner: public Scanner {
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
        if ((39 <= c && c <= 39)) {
            goto s2;
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
    s2:
        c = in.get();
        if (c == eof) {
            goto se;
        }
        lexeme += c;
        checkpoint.push_back(c);
        if ((32 <= c && c <= 38) || (40 <= c && c <= 91) || (93 <= c && c <= 110) || (110 <= c && c <= 110) || (110 <= c && c <= 116) || (116 <= c && c <= 116) || (116 <= c && c <= 126)) {
            goto s3;
        }
        if ((92 <= c && c <= 92)) {
            goto s4;
        }
        goto se;
    s3:
        c = in.get();
        if (c == eof) {
            goto se;
        }
        lexeme += c;
        checkpoint.push_back(c);
        if ((39 <= c && c <= 39)) {
            goto s1;
        }
        goto se;
    s4:
        c = in.get();
        if (c == eof) {
            goto se;
        }
        lexeme += c;
        checkpoint.push_back(c);
        if ((39 <= c && c <= 39) || (92 <= c && c <= 92) || (110 <= c && c <= 110) || (116 <= c && c <= 116)) {
            goto s3;
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

struct identifierScanner: public Scanner {
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
        if ((65 <= c && c <= 90) || (95 <= c && c <= 95) || (97 <= c && c <= 122)) {
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

struct numberScanner: public Scanner {
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
        if ((49 <= c && c <= 57)) {
            goto s1;
        }
        if ((48 <= c && c <= 48)) {
            goto s5;
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
        c = in.get();
        if (c == eof) {
            goto se;
        }
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
        c = in.get();
        if (c == eof) {
            goto se;
        }
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
        c = in.get();
        if (c == eof) {
            goto se;
        }
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
        c = in.get();
        if (c == eof) {
            goto se;
        }
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
        c = in.get();
        if (c == eof) {
            goto se;
        }
        lexeme += c;
        checkpoint.clear();
        checkpoint.push_back(c);
        goto se;
    s7:
        c = in.get();
        if (c == eof) {
            goto se;
        }
        lexeme += c;
        checkpoint.push_back(c);
        if ((48 <= c && c <= 48) || (49 <= c && c <= 57)) {
            goto s3;
        }
        goto se;
    s8:
        c = in.get();
        if (c == eof) {
            goto se;
        }
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

struct stringScanner: public Scanner {
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
        if ((34 <= c && c <= 34)) {
            goto s2;
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
    s2:
        c = in.get();
        if (c == eof) {
            goto se;
        }
        lexeme += c;
        checkpoint.push_back(c);
        if ((34 <= c && c <= 34)) {
            goto s1;
        }
        if ((32 <= c && c <= 33) || (35 <= c && c <= 91) || (93 <= c && c <= 126)) {
            goto s2;
        }
        if ((92 <= c && c <= 92)) {
            goto s3;
        }
        goto se;
    s3:
        c = in.get();
        if (c == eof) {
            goto se;
        }
        lexeme += c;
        checkpoint.push_back(c);
        if ((32 <= c && c <= 33) || (34 <= c && c <= 34) || (35 <= c && c <= 91) || (92 <= c && c <= 92) || (93 <= c && c <= 126)) {
            goto s2;
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
        scanners.push_back(std::make_shared<_ignoreScanner>(Token::_ignore));
        scanners.push_back(std::make_shared<characterScanner>(Token::character));
        scanners.push_back(std::make_shared<identifierScanner>(Token::identifier));
        scanners.push_back(std::make_shared<numberScanner>(Token::number));
        scanners.push_back(std::make_shared<stringScanner>(Token::string));
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

