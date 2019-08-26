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
    std::vector<int> checkpoint;

    Scanner(Token token) : token(token), checkpoint({0}) {}
    virtual ~Scanner() {}
    virtual bool next(int) = 0;

    int state() const
    {
        return checkpoint.back();
    }

    int backtrack(bool clear=false)
    {
        int steps = checkpoint.size();
        if (clear) {
            checkpoint.clear();
            checkpoint.push_back(0);
        }
        return steps;
    }
};

struct EmptyScanner: public Scanner {
    //using Scanner::Scanner;
    EmptyScanner() : Scanner(Token::empty) {}
    bool next(int c)
    {
        int eof = std::char_traits<char>::eof();
        if (c == eof && state() != -1) {
            checkpoint.push_back(-1);
        }
        switch (state()) {
        case 0:
            checkpoint.clear();
            checkpoint.push_back(-1);
            return false;
        default:
            return false;
        }
    }
};

struct IdentifierScanner: public Scanner {
    //using Scanner::Scanner;
    IdentifierScanner() : Scanner(Token::identifier) {}
    bool next(int c)
    {
        int eof = std::char_traits<char>::eof();
        if (c == eof && state() != -1) {
            checkpoint.push_back(-1);
        }
        switch (state()) {
        case 0:
            if (65 <= c && c <= 90) {
                checkpoint.push_back(1);
                return true;
            }
            if (95 <= c && c <= 95) {
                checkpoint.push_back(1);
                return true;
            }
            if (97 <= c && c <= 122) {
                checkpoint.push_back(1);
                return true;
            }
            checkpoint.push_back(-1);
            return false;
        case 1:
            checkpoint.clear();
            if (48 <= c && c <= 57) {
                checkpoint.push_back(1);
                return true;
            }
            if (65 <= c && c <= 90) {
                checkpoint.push_back(1);
                return true;
            }
            if (95 <= c && c <= 95) {
                checkpoint.push_back(1);
                return true;
            }
            if (97 <= c && c <= 122) {
                checkpoint.push_back(1);
                return true;
            }
            checkpoint.push_back(-1);
            return false;
        default:
            return false;
        }
    }
};

struct WhitespaceScanner: public Scanner {
    //using Scanner::Scanner;
    WhitespaceScanner() : Scanner(Token::whitespace) {}
    bool next(int c)
    {
        int eof = std::char_traits<char>::eof();
        if (c == eof && state() != -1) {
            checkpoint.push_back(-1);
        }
        switch (state()) {
        case 0:
            if (9 <= c && c <= 9) {
                checkpoint.push_back(1);
                return true;
            }
            if (10 <= c && c <= 10) {
                checkpoint.push_back(1);
                return true;
            }
            if (32 <= c && c <= 32) {
                checkpoint.push_back(1);
                return true;
            }
            checkpoint.push_back(-1);
            return false;
        case 1:
            checkpoint.clear();
            checkpoint.push_back(-1);
            return false;
        default:
            return false;
        }
    }
};

struct IntegerScanner: public Scanner {
    //using Scanner::Scanner;
    IntegerScanner() : Scanner(Token::integer) {}
    bool next(int c)
    {
        int eof = std::char_traits<char>::eof();
        if (c == eof && state() != -1) {
            checkpoint.push_back(-1);
        }
        switch (state()) {
        case 0:
            if (48 <= c && c <= 48) {
                checkpoint.push_back(1);
                return true;
            }
            if (49 <= c && c <= 57) {
                checkpoint.push_back(2);
                return true;
            }
            checkpoint.push_back(-1);
            return false;
        case 1:
            checkpoint.clear();
            checkpoint.push_back(-1);
            return false;
        case 2:
            checkpoint.clear();
            if (48 <= c && c <= 48) {
                checkpoint.push_back(2);
                return true;
            }
            if (49 <= c && c <= 57) {
                checkpoint.push_back(2);
                return true;
            }
            checkpoint.push_back(-1);
            return false;
        default:
            return false;
        }
    }
};

struct NumberScanner: public Scanner {
    //using Scanner::Scanner;
    NumberScanner() : Scanner(Token::number) {}
    bool next(int c)
    {
        int eof = std::char_traits<char>::eof();
        if (c == eof && state() != -1) {
            checkpoint.push_back(-1);
        }
        switch (state()) {
        case 0:
            if (48 <= c && c <= 48) {
                checkpoint.push_back(5);
                return true;
            }
            if (49 <= c && c <= 57) {
                checkpoint.push_back(1);
                return true;
            }
            checkpoint.push_back(-1);
            return false;
        case 5:
            checkpoint.clear();
            if (46 <= c && c <= 46) {
                checkpoint.push_back(7);
                return true;
            }
            if (69 <= c && c <= 69) {
                checkpoint.push_back(2);
                return true;
            }
            if (101 <= c && c <= 101) {
                checkpoint.push_back(2);
                return true;
            }
            checkpoint.push_back(-1);
            return false;
        case 1:
            checkpoint.clear();
            if (46 <= c && c <= 46) {
                checkpoint.push_back(7);
                return true;
            }
            if (48 <= c && c <= 48) {
                checkpoint.push_back(1);
                return true;
            }
            if (49 <= c && c <= 57) {
                checkpoint.push_back(1);
                return true;
            }
            if (69 <= c && c <= 69) {
                checkpoint.push_back(2);
                return true;
            }
            if (101 <= c && c <= 101) {
                checkpoint.push_back(2);
                return true;
            }
            checkpoint.push_back(-1);
            return false;
        case 7:
            if (48 <= c && c <= 48) {
                checkpoint.push_back(3);
                return true;
            }
            if (49 <= c && c <= 57) {
                checkpoint.push_back(3);
                return true;
            }
            checkpoint.push_back(-1);
            return false;
        case 2:
            if (43 <= c && c <= 43) {
                checkpoint.push_back(4);
                return true;
            }
            if (45 <= c && c <= 45) {
                checkpoint.push_back(4);
                return true;
            }
            if (48 <= c && c <= 48) {
                checkpoint.push_back(6);
                return true;
            }
            if (49 <= c && c <= 57) {
                checkpoint.push_back(8);
                return true;
            }
            checkpoint.push_back(-1);
            return false;
        case 4:
            if (48 <= c && c <= 48) {
                checkpoint.push_back(6);
                return true;
            }
            if (49 <= c && c <= 57) {
                checkpoint.push_back(8);
                return true;
            }
            checkpoint.push_back(-1);
            return false;
        case 6:
            checkpoint.clear();
            checkpoint.push_back(-1);
            return false;
        case 8:
            checkpoint.clear();
            if (48 <= c && c <= 48) {
                checkpoint.push_back(8);
                return true;
            }
            if (49 <= c && c <= 57) {
                checkpoint.push_back(8);
                return true;
            }
            checkpoint.push_back(-1);
            return false;
        case 3:
            checkpoint.clear();
            if (48 <= c && c <= 48) {
                checkpoint.push_back(3);
                return true;
            }
            if (49 <= c && c <= 57) {
                checkpoint.push_back(3);
                return true;
            }
            if (69 <= c && c <= 69) {
                checkpoint.push_back(2);
                return true;
            }
            if (101 <= c && c <= 101) {
                checkpoint.push_back(2);
                return true;
            }
            checkpoint.push_back(-1);
            return false;
        default:
            return false;
        }
    }
};

struct CharacterScanner: public Scanner {
    //using Scanner::Scanner;
    CharacterScanner() : Scanner(Token::character) {}
    bool next(int c)
    {
        int eof = std::char_traits<char>::eof();
        if (c == eof && state() != -1) {
            checkpoint.push_back(-1);
        }
        switch (state()) {
        case 0:
            if (39 <= c && c <= 39) {
                checkpoint.push_back(2);
                return true;
            }
            checkpoint.push_back(-1);
            return false;
        case 2:
            if (32 <= c && c <= 38) {
                checkpoint.push_back(3);
                return true;
            }
            if (40 <= c && c <= 91) {
                checkpoint.push_back(3);
                return true;
            }
            if (92 <= c && c <= 92) {
                checkpoint.push_back(4);
                return true;
            }
            if (93 <= c && c <= 126) {
                checkpoint.push_back(3);
                return true;
            }
            checkpoint.push_back(-1);
            return false;
        case 1:
            checkpoint.clear();
            checkpoint.push_back(-1);
            return false;
        case 3:
            if (39 <= c && c <= 39) {
                checkpoint.push_back(1);
                return true;
            }
            checkpoint.push_back(-1);
            return false;
        case 4:
            if (39 <= c && c <= 39) {
                checkpoint.push_back(3);
                return true;
            }
            if (92 <= c && c <= 92) {
                checkpoint.push_back(3);
                return true;
            }
            if (93 <= c && c <= 126) {
                checkpoint.push_back(3);
                return true;
            }
            checkpoint.push_back(-1);
            return false;
        default:
            return false;
        }
    }
};

struct StringScanner: public Scanner {
    //using Scanner::Scanner;
    StringScanner() : Scanner(Token::string) {}
    bool next(int c)
    {
        int eof = std::char_traits<char>::eof();
        if (c == eof && state() != -1) {
            checkpoint.push_back(-1);
        }
        switch (state()) {
        case 0:
            if (34 <= c && c <= 34) {
                checkpoint.push_back(2);
                return true;
            }
            checkpoint.push_back(-1);
            return false;
        case 2:
            if (32 <= c && c <= 33) {
                checkpoint.push_back(2);
                return true;
            }
            if (34 <= c && c <= 34) {
                checkpoint.push_back(1);
                return true;
            }
            if (35 <= c && c <= 91) {
                checkpoint.push_back(2);
                return true;
            }
            if (92 <= c && c <= 92) {
                checkpoint.push_back(3);
                return true;
            }
            if (93 <= c && c <= 126) {
                checkpoint.push_back(2);
                return true;
            }
            checkpoint.push_back(-1);
            return false;
        case 1:
            checkpoint.clear();
            checkpoint.push_back(-1);
            return false;
        case 3:
            if (32 <= c && c <= 33) {
                checkpoint.push_back(2);
                return true;
            }
            if (34 <= c && c <= 34) {
                checkpoint.push_back(2);
                return true;
            }
            if (35 <= c && c <= 91) {
                checkpoint.push_back(2);
                return true;
            }
            if (92 <= c && c <= 92) {
                checkpoint.push_back(2);
                return true;
            }
            if (93 <= c && c <= 126) {
                checkpoint.push_back(2);
                return true;
            }
            checkpoint.push_back(-1);
            return false;
        default:
            return false;
        }
    }
};


