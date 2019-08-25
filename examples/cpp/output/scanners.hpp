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
    empty,
    identifier,
    whitespace,
    integer,
    number,
    character,
    string,
};

std::ostream& operator<<(std::ostream& out, Token token)
{
    switch (token) {
    case Token::_other:
        return out << "_other";
    case Token::_empty:
        return out << "_empty";
    case Token::empty:
        return out << "empty";
    case Token::identifier:
        return out << "identifier";
    case Token::whitespace:
        return out << "whitespace";
    case Token::integer:
        return out << "integer";
    case Token::number:
        return out << "number";
    case Token::character:
        return out << "character";
    case Token::string:
        return out << "string";
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

struct EmptyScanner: public Scanner {
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

struct IdentifierScanner: public Scanner {
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
        if (65 <= c && c <= 90) {
            goto s1;
        }
        if (95 <= c && c <= 95) {
            goto s1;
        }
        if (97 <= c && c <= 122) {
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
        if (48 <= c && c <= 57) {
            goto s1;
        }
        if (65 <= c && c <= 90) {
            goto s1;
        }
        if (95 <= c && c <= 95) {
            goto s1;
        }
        if (97 <= c && c <= 122) {
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

struct WhitespaceScanner: public Scanner {
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
        if (9 <= c && c <= 9) {
            goto s1;
        }
        if (10 <= c && c <= 10) {
            goto s1;
        }
        if (32 <= c && c <= 32) {
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

struct IntegerScanner: public Scanner {
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
        if (48 <= c && c <= 48) {
            goto s1;
        }
        if (49 <= c && c <= 57) {
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
        checkpoint.clear();
        checkpoint.push_back(c);
        if (48 <= c && c <= 48) {
            goto s2;
        }
        if (49 <= c && c <= 57) {
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

struct NumberScanner: public Scanner {
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
        if (48 <= c && c <= 48) {
            goto s5;
        }
        if (49 <= c && c <= 57) {
            goto s1;
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
        if (46 <= c && c <= 46) {
            goto s7;
        }
        if (69 <= c && c <= 69) {
            goto s2;
        }
        if (101 <= c && c <= 101) {
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
        if (46 <= c && c <= 46) {
            goto s7;
        }
        if (48 <= c && c <= 48) {
            goto s1;
        }
        if (49 <= c && c <= 57) {
            goto s1;
        }
        if (69 <= c && c <= 69) {
            goto s2;
        }
        if (101 <= c && c <= 101) {
            goto s2;
        }
        goto se;
    s7:
        c = in.get();
        if (c == eof) {
            goto se;
        }
        lexeme += c;
        checkpoint.push_back(c);
        if (48 <= c && c <= 48) {
            goto s3;
        }
        if (49 <= c && c <= 57) {
            goto s3;
        }
        goto se;
    s2:
        c = in.get();
        if (c == eof) {
            goto se;
        }
        lexeme += c;
        checkpoint.push_back(c);
        if (43 <= c && c <= 43) {
            goto s4;
        }
        if (45 <= c && c <= 45) {
            goto s4;
        }
        if (48 <= c && c <= 48) {
            goto s6;
        }
        if (49 <= c && c <= 57) {
            goto s8;
        }
        goto se;
    s4:
        c = in.get();
        if (c == eof) {
            goto se;
        }
        lexeme += c;
        checkpoint.push_back(c);
        if (48 <= c && c <= 48) {
            goto s6;
        }
        if (49 <= c && c <= 57) {
            goto s8;
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
    s8:
        c = in.get();
        if (c == eof) {
            goto se;
        }
        lexeme += c;
        checkpoint.clear();
        checkpoint.push_back(c);
        if (48 <= c && c <= 48) {
            goto s8;
        }
        if (49 <= c && c <= 57) {
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
        if (48 <= c && c <= 48) {
            goto s3;
        }
        if (49 <= c && c <= 57) {
            goto s3;
        }
        if (69 <= c && c <= 69) {
            goto s2;
        }
        if (101 <= c && c <= 101) {
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

struct CharacterScanner: public Scanner {
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
        if (39 <= c && c <= 39) {
            goto s2;
        }
        goto se;
    s2:
        c = in.get();
        if (c == eof) {
            goto se;
        }
        lexeme += c;
        checkpoint.push_back(c);
        if (32 <= c && c <= 38) {
            goto s3;
        }
        if (40 <= c && c <= 91) {
            goto s3;
        }
        if (92 <= c && c <= 92) {
            goto s4;
        }
        if (93 <= c && c <= 126) {
            goto s3;
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
    s3:
        c = in.get();
        if (c == eof) {
            goto se;
        }
        lexeme += c;
        checkpoint.push_back(c);
        if (39 <= c && c <= 39) {
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
        if (39 <= c && c <= 39) {
            goto s3;
        }
        if (92 <= c && c <= 92) {
            goto s3;
        }
        if (93 <= c && c <= 126) {
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

struct StringScanner: public Scanner {
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
        if (34 <= c && c <= 34) {
            goto s2;
        }
        goto se;
    s2:
        c = in.get();
        if (c == eof) {
            goto se;
        }
        lexeme += c;
        checkpoint.push_back(c);
        if (32 <= c && c <= 33) {
            goto s2;
        }
        if (34 <= c && c <= 34) {
            goto s1;
        }
        if (35 <= c && c <= 91) {
            goto s2;
        }
        if (92 <= c && c <= 92) {
            goto s3;
        }
        if (93 <= c && c <= 126) {
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
    s3:
        c = in.get();
        if (c == eof) {
            goto se;
        }
        lexeme += c;
        checkpoint.push_back(c);
        if (32 <= c && c <= 33) {
            goto s2;
        }
        if (34 <= c && c <= 34) {
            goto s2;
        }
        if (35 <= c && c <= 91) {
            goto s2;
        }
        if (92 <= c && c <= 92) {
            goto s2;
        }
        if (93 <= c && c <= 126) {
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
        //scanners.push_back(std::make_shared<_IgnoreScanner>(Token::_IgnoreScanner));
        scanners.push_back(std::make_shared<EmptyScanner >(Token::empty));
        scanners.push_back(std::make_shared<IdentifierScanner >(Token::identifier));
        scanners.push_back(std::make_shared<WhitespaceScanner >(Token::whitespace));
        scanners.push_back(std::make_shared<IntegerScanner >(Token::integer));
        scanners.push_back(std::make_shared<NumberScanner >(Token::number));
        scanners.push_back(std::make_shared<CharacterScanner >(Token::character));
        scanners.push_back(std::make_shared<StringScanner >(Token::string));
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
