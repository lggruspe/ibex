#pragma once
#include <iostream>
#include <string>
#include <utility>
#include <vector>

struct floatScanner {
    std::istream* in;
    floatScanner(std::istream& in)
    {
        this->in = &in;
    }

    bool contains(const char *s, char c)
    {
        auto it = s;
        while (*it && *it != c) {
            ++it;
        }
        return *it == c;
    }

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

    std::pair<std::string, std::string> operator()()
    {
        char c;
        std::string cat;
        std::vector<char> checkpoint;
        std::string lexeme;
        bool accept = false;
        goto s0;
    s0:
        in->get(c);
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
        in->get(c);
        lexeme += c;
        checkpoint.clear();
        accept = true;
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
        in->get(c);
        lexeme += c;
        checkpoint.clear();
        accept = true;
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
        in->get(c);
        lexeme += c;
        checkpoint.clear();
        accept = true;
        checkpoint.push_back(c);
        cat = category(c);
        goto se;

    s4:
        in->get(c);
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
        in->get(c);
        lexeme += c;
        checkpoint.clear();
        accept = true;
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
        in->get(c);
        lexeme += c;
        checkpoint.clear();
        accept = true;
        checkpoint.push_back(c);
        cat = category(c);
        if (cat == "nonzero" || cat == "zero") {
            goto s6;
        }
        goto se;

    s7:
        in->get(c);
        lexeme += c;
        checkpoint.push_back(c);
        cat = category(c);
        if (cat == "nonzero" || cat == "zero") {
            goto s2;
        }
        goto se;

    s8:
        in->get(c);
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
            in->putback(c);
            lexeme.pop_back();
        }
        return std::pair<std::string, std::string>("float", lexeme);
    }
};

