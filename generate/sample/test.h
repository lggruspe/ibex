#pragma once
#include <iostream>
#include <string>
#include <utility>
#include <vector>

class Scanner {
protected:
    bool contains(const char* s, char c)
    {
        auto it = s;
        while (*it && *it != c) {
            ++it;
        }
        return *it == c;
    }

public:
    std::string token;
    Scanner(const std::string& token) : token(token) {}
    virtual std::string operator()(std::istream&) = 0;
};

class integerScanner: public Scanner {
    std::string category(char c)
    {
        if (contains("123456789", c)) {
            return "nonzero";
        }
        if (contains("0", c)) {
            return "zero";
        }
        return "other";
    }

public:
    using Scanner::Scanner;
    std::string operator()(std::istream& in)
    {
        char c;
        std::string cat;
        std::vector<char> checkpoint;
        std::string lexeme;
        goto s0;
    s0:
        in.get(c);
        lexeme += c;
        checkpoint.push_back(c);
        cat = category(c);
        if (cat == "nonzero") {
            goto s1;
        }
        if (cat == "zero") {
            goto s2;
        }
        goto se;

    s1:
        in.get(c);
        lexeme += c;
        checkpoint.clear();
        checkpoint.push_back(c);
        cat = category(c);
        if (cat == "nonzero" || cat == "zero") {
            goto s1;
        }
        goto se;

    s2:
        in.get(c);
        lexeme += c;
        checkpoint.clear();
        checkpoint.push_back(c);
        cat = category(c);
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
    std::string category(char c)
    {
        if (contains(".", c)) {
            return ".";
        }
        if (contains("eE", c)) {
            return "exponent";
        }
        if (contains("123456789", c)) {
            return "nonzero";
        }
        if (contains("+-", c)) {
            return "sign";
        }
        if (contains("0", c)) {
            return "zero";
        }
        return "other";
    }

public:
    using Scanner::Scanner;
    std::string operator()(std::istream& in)
    {
        char c;
        std::string cat;
        std::vector<char> checkpoint;
        std::string lexeme;
        goto s0;
    s0:
        in.get(c);
        lexeme += c;
        checkpoint.push_back(c);
        cat = category(c);
        if (cat == "zero") {
            goto s1;
        }
        if (cat == "nonzero") {
            goto s5;
        }
        goto se;

    s1:
        in.get(c);
        lexeme += c;
        checkpoint.clear();
        checkpoint.push_back(c);
        cat = category(c);
        if (cat == ".") {
            goto s7;
        }
        if (cat == "exponent") {
            goto s8;
        }
        goto se;

    s2:
        in.get(c);
        lexeme += c;
        checkpoint.clear();
        checkpoint.push_back(c);
        cat = category(c);
        if (cat == "nonzero" || cat == "zero") {
            goto s2;
        }
        if (cat == "exponent") {
            goto s8;
        }
        goto se;

    s3:
        in.get(c);
        lexeme += c;
        checkpoint.clear();
        checkpoint.push_back(c);
        cat = category(c);
        goto se;

    s4:
        in.get(c);
        lexeme += c;
        checkpoint.push_back(c);
        cat = category(c);
        if (cat == "zero") {
            goto s3;
        }
        if (cat == "nonzero") {
            goto s6;
        }
        goto se;

    s5:
        in.get(c);
        lexeme += c;
        checkpoint.clear();
        checkpoint.push_back(c);
        cat = category(c);
        if (cat == "nonzero" || cat == "zero") {
            goto s5;
        }
        if (cat == ".") {
            goto s7;
        }
        if (cat == "exponent") {
            goto s8;
        }
        goto se;

    s6:
        in.get(c);
        lexeme += c;
        checkpoint.clear();
        checkpoint.push_back(c);
        cat = category(c);
        if (cat == "nonzero" || cat == "zero") {
            goto s6;
        }
        goto se;

    s7:
        in.get(c);
        lexeme += c;
        checkpoint.push_back(c);
        cat = category(c);
        if (cat == "nonzero" || cat == "zero") {
            goto s2;
        }
        goto se;

    s8:
        in.get(c);
        lexeme += c;
        checkpoint.push_back(c);
        cat = category(c);
        if (cat == "zero") {
            goto s3;
        }
        if (cat == "sign") {
            goto s4;
        }
        if (cat == "nonzero") {
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
    std::string category(char c)
    {
        if (contains("1234567890", c)) {
            return "digit";
        }
        if (contains("_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", c)) {
            return "letter";
        }
        return "other";
    }

public:
    using Scanner::Scanner;
    std::string operator()(std::istream& in)
    {
        char c;
        std::string cat;
        std::vector<char> checkpoint;
        std::string lexeme;
        goto s0;
    s0:
        in.get(c);
        lexeme += c;
        checkpoint.push_back(c);
        cat = category(c);
        if (cat == "letter") {
            goto s1;
        }
        goto se;

    s1:
        in.get(c);
        lexeme += c;
        checkpoint.clear();
        checkpoint.push_back(c);
        cat = category(c);
        if (cat == "digit" || cat == "letter") {
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

    std::pair<std::string, std::string> operator()()
    {
        for (auto& p: scanners) {
            Scanner& scanner = *p;
            std::string lexeme = scanner(*in);
            if (!lexeme.empty()) {
                return std::pair<std::string, std::string>(scanner.token, lexeme);
            }
        }
        throw "syntax error";
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

