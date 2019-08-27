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
    number,
    character,
    string,
    dot,
    lparen,
    rparen,
    comma,
    star,
    equal,
    lbrace,
    rbrace,
    colon,
    lbracket,
    rbracket,
    plus,
    minus,
    slash,
    lessthan,
    greaterthan,
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
    case Token::number:
        return out << "number";
    case Token::character:
        return out << "character";
    case Token::string:
        return out << "string";
    case Token::dot:
        return out << "dot";
    case Token::lparen:
        return out << "lparen";
    case Token::rparen:
        return out << "rparen";
    case Token::comma:
        return out << "comma";
    case Token::star:
        return out << "star";
    case Token::equal:
        return out << "equal";
    case Token::lbrace:
        return out << "lbrace";
    case Token::rbrace:
        return out << "rbrace";
    case Token::colon:
        return out << "colon";
    case Token::lbracket:
        return out << "lbracket";
    case Token::rbracket:
        return out << "rbracket";
    case Token::plus:
        return out << "plus";
    case Token::minus:
        return out << "minus";
    case Token::slash:
        return out << "slash";
    case Token::lessthan:
        return out << "lessthan";
    case Token::greaterthan:
        return out << "greaterthan";
    default:
        return out;
    }
}

struct Scanner {
    Token token;
    std::vector<int> checkpoint;
    int accept;

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
    EmptyScanner() : Scanner(Token::empty)
    {
        accept = 0;
    }

    bool next(int c)
    {
        int eof = std::char_traits<char>::eof();
        if (c == eof && state() != -1) {
            checkpoint.push_back(-1);
        }
        switch (state()) {
        case 0:
            accept = state();
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
    IdentifierScanner() : Scanner(Token::identifier)
    {
        accept = -1;
    }

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
            accept = state();
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
    WhitespaceScanner() : Scanner(Token::whitespace)
    {
        accept = -1;
    }

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
            accept = state();
            checkpoint.clear();
            checkpoint.push_back(-1);
            return false;
        default:
            return false;
        }
    }
};

struct NumberScanner: public Scanner {
    //using Scanner::Scanner;
    NumberScanner() : Scanner(Token::number)
    {
        accept = -1;
    }

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
            accept = state();
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
            accept = state();
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
            accept = state();
            checkpoint.clear();
            checkpoint.push_back(-1);
            return false;
        case 8:
            accept = state();
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
            accept = state();
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
    CharacterScanner() : Scanner(Token::character)
    {
        accept = -1;
    }

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
            accept = state();
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
    StringScanner() : Scanner(Token::string)
    {
        accept = -1;
    }

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
            accept = state();
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

struct DotScanner: public Scanner {
    //using Scanner::Scanner;
    DotScanner() : Scanner(Token::dot)
    {
        accept = -1;
    }

    bool next(int c)
    {
        int eof = std::char_traits<char>::eof();
        if (c == eof && state() != -1) {
            checkpoint.push_back(-1);
        }
        switch (state()) {
        case 0:
            if (46 <= c && c <= 46) {
                checkpoint.push_back(1);
                return true;
            }
            checkpoint.push_back(-1);
            return false;
        case 1:
            accept = state();
            checkpoint.clear();
            checkpoint.push_back(-1);
            return false;
        default:
            return false;
        }
    }
};

struct LparenScanner: public Scanner {
    //using Scanner::Scanner;
    LparenScanner() : Scanner(Token::lparen)
    {
        accept = -1;
    }

    bool next(int c)
    {
        int eof = std::char_traits<char>::eof();
        if (c == eof && state() != -1) {
            checkpoint.push_back(-1);
        }
        switch (state()) {
        case 0:
            if (40 <= c && c <= 40) {
                checkpoint.push_back(1);
                return true;
            }
            checkpoint.push_back(-1);
            return false;
        case 1:
            accept = state();
            checkpoint.clear();
            checkpoint.push_back(-1);
            return false;
        default:
            return false;
        }
    }
};

struct RparenScanner: public Scanner {
    //using Scanner::Scanner;
    RparenScanner() : Scanner(Token::rparen)
    {
        accept = -1;
    }

    bool next(int c)
    {
        int eof = std::char_traits<char>::eof();
        if (c == eof && state() != -1) {
            checkpoint.push_back(-1);
        }
        switch (state()) {
        case 0:
            if (41 <= c && c <= 41) {
                checkpoint.push_back(1);
                return true;
            }
            checkpoint.push_back(-1);
            return false;
        case 1:
            accept = state();
            checkpoint.clear();
            checkpoint.push_back(-1);
            return false;
        default:
            return false;
        }
    }
};

struct CommaScanner: public Scanner {
    //using Scanner::Scanner;
    CommaScanner() : Scanner(Token::comma)
    {
        accept = -1;
    }

    bool next(int c)
    {
        int eof = std::char_traits<char>::eof();
        if (c == eof && state() != -1) {
            checkpoint.push_back(-1);
        }
        switch (state()) {
        case 0:
            if (44 <= c && c <= 44) {
                checkpoint.push_back(1);
                return true;
            }
            checkpoint.push_back(-1);
            return false;
        case 1:
            accept = state();
            checkpoint.clear();
            checkpoint.push_back(-1);
            return false;
        default:
            return false;
        }
    }
};

struct StarScanner: public Scanner {
    //using Scanner::Scanner;
    StarScanner() : Scanner(Token::star)
    {
        accept = -1;
    }

    bool next(int c)
    {
        int eof = std::char_traits<char>::eof();
        if (c == eof && state() != -1) {
            checkpoint.push_back(-1);
        }
        switch (state()) {
        case 0:
            if (42 <= c && c <= 42) {
                checkpoint.push_back(1);
                return true;
            }
            checkpoint.push_back(-1);
            return false;
        case 1:
            accept = state();
            checkpoint.clear();
            checkpoint.push_back(-1);
            return false;
        default:
            return false;
        }
    }
};

struct EqualScanner: public Scanner {
    //using Scanner::Scanner;
    EqualScanner() : Scanner(Token::equal)
    {
        accept = -1;
    }

    bool next(int c)
    {
        int eof = std::char_traits<char>::eof();
        if (c == eof && state() != -1) {
            checkpoint.push_back(-1);
        }
        switch (state()) {
        case 0:
            if (61 <= c && c <= 61) {
                checkpoint.push_back(1);
                return true;
            }
            checkpoint.push_back(-1);
            return false;
        case 1:
            accept = state();
            checkpoint.clear();
            checkpoint.push_back(-1);
            return false;
        default:
            return false;
        }
    }
};

struct LbraceScanner: public Scanner {
    //using Scanner::Scanner;
    LbraceScanner() : Scanner(Token::lbrace)
    {
        accept = -1;
    }

    bool next(int c)
    {
        int eof = std::char_traits<char>::eof();
        if (c == eof && state() != -1) {
            checkpoint.push_back(-1);
        }
        switch (state()) {
        case 0:
            if (123 <= c && c <= 123) {
                checkpoint.push_back(1);
                return true;
            }
            checkpoint.push_back(-1);
            return false;
        case 1:
            accept = state();
            checkpoint.clear();
            checkpoint.push_back(-1);
            return false;
        default:
            return false;
        }
    }
};

struct RbraceScanner: public Scanner {
    //using Scanner::Scanner;
    RbraceScanner() : Scanner(Token::rbrace)
    {
        accept = -1;
    }

    bool next(int c)
    {
        int eof = std::char_traits<char>::eof();
        if (c == eof && state() != -1) {
            checkpoint.push_back(-1);
        }
        switch (state()) {
        case 0:
            if (125 <= c && c <= 125) {
                checkpoint.push_back(1);
                return true;
            }
            checkpoint.push_back(-1);
            return false;
        case 1:
            accept = state();
            checkpoint.clear();
            checkpoint.push_back(-1);
            return false;
        default:
            return false;
        }
    }
};

struct ColonScanner: public Scanner {
    //using Scanner::Scanner;
    ColonScanner() : Scanner(Token::colon)
    {
        accept = -1;
    }

    bool next(int c)
    {
        int eof = std::char_traits<char>::eof();
        if (c == eof && state() != -1) {
            checkpoint.push_back(-1);
        }
        switch (state()) {
        case 0:
            if (58 <= c && c <= 58) {
                checkpoint.push_back(1);
                return true;
            }
            checkpoint.push_back(-1);
            return false;
        case 1:
            accept = state();
            checkpoint.clear();
            checkpoint.push_back(-1);
            return false;
        default:
            return false;
        }
    }
};

struct LbracketScanner: public Scanner {
    //using Scanner::Scanner;
    LbracketScanner() : Scanner(Token::lbracket)
    {
        accept = -1;
    }

    bool next(int c)
    {
        int eof = std::char_traits<char>::eof();
        if (c == eof && state() != -1) {
            checkpoint.push_back(-1);
        }
        switch (state()) {
        case 0:
            if (91 <= c && c <= 91) {
                checkpoint.push_back(1);
                return true;
            }
            checkpoint.push_back(-1);
            return false;
        case 1:
            accept = state();
            checkpoint.clear();
            checkpoint.push_back(-1);
            return false;
        default:
            return false;
        }
    }
};

struct RbracketScanner: public Scanner {
    //using Scanner::Scanner;
    RbracketScanner() : Scanner(Token::rbracket)
    {
        accept = -1;
    }

    bool next(int c)
    {
        int eof = std::char_traits<char>::eof();
        if (c == eof && state() != -1) {
            checkpoint.push_back(-1);
        }
        switch (state()) {
        case 0:
            if (93 <= c && c <= 93) {
                checkpoint.push_back(1);
                return true;
            }
            checkpoint.push_back(-1);
            return false;
        case 1:
            accept = state();
            checkpoint.clear();
            checkpoint.push_back(-1);
            return false;
        default:
            return false;
        }
    }
};

struct PlusScanner: public Scanner {
    //using Scanner::Scanner;
    PlusScanner() : Scanner(Token::plus)
    {
        accept = -1;
    }

    bool next(int c)
    {
        int eof = std::char_traits<char>::eof();
        if (c == eof && state() != -1) {
            checkpoint.push_back(-1);
        }
        switch (state()) {
        case 0:
            if (43 <= c && c <= 43) {
                checkpoint.push_back(1);
                return true;
            }
            checkpoint.push_back(-1);
            return false;
        case 1:
            accept = state();
            checkpoint.clear();
            checkpoint.push_back(-1);
            return false;
        default:
            return false;
        }
    }
};

struct MinusScanner: public Scanner {
    //using Scanner::Scanner;
    MinusScanner() : Scanner(Token::minus)
    {
        accept = -1;
    }

    bool next(int c)
    {
        int eof = std::char_traits<char>::eof();
        if (c == eof && state() != -1) {
            checkpoint.push_back(-1);
        }
        switch (state()) {
        case 0:
            if (45 <= c && c <= 45) {
                checkpoint.push_back(1);
                return true;
            }
            checkpoint.push_back(-1);
            return false;
        case 1:
            accept = state();
            checkpoint.clear();
            checkpoint.push_back(-1);
            return false;
        default:
            return false;
        }
    }
};

struct SlashScanner: public Scanner {
    //using Scanner::Scanner;
    SlashScanner() : Scanner(Token::slash)
    {
        accept = -1;
    }

    bool next(int c)
    {
        int eof = std::char_traits<char>::eof();
        if (c == eof && state() != -1) {
            checkpoint.push_back(-1);
        }
        switch (state()) {
        case 0:
            if (47 <= c && c <= 47) {
                checkpoint.push_back(1);
                return true;
            }
            checkpoint.push_back(-1);
            return false;
        case 1:
            accept = state();
            checkpoint.clear();
            checkpoint.push_back(-1);
            return false;
        default:
            return false;
        }
    }
};

struct LessthanScanner: public Scanner {
    //using Scanner::Scanner;
    LessthanScanner() : Scanner(Token::lessthan)
    {
        accept = -1;
    }

    bool next(int c)
    {
        int eof = std::char_traits<char>::eof();
        if (c == eof && state() != -1) {
            checkpoint.push_back(-1);
        }
        switch (state()) {
        case 0:
            if (60 <= c && c <= 60) {
                checkpoint.push_back(1);
                return true;
            }
            checkpoint.push_back(-1);
            return false;
        case 1:
            accept = state();
            checkpoint.clear();
            checkpoint.push_back(-1);
            return false;
        default:
            return false;
        }
    }
};

struct GreaterthanScanner: public Scanner {
    //using Scanner::Scanner;
    GreaterthanScanner() : Scanner(Token::greaterthan)
    {
        accept = -1;
    }

    bool next(int c)
    {
        int eof = std::char_traits<char>::eof();
        if (c == eof && state() != -1) {
            checkpoint.push_back(-1);
        }
        switch (state()) {
        case 0:
            if (62 <= c && c <= 62) {
                checkpoint.push_back(1);
                return true;
            }
            checkpoint.push_back(-1);
            return false;
        case 1:
            accept = state();
            checkpoint.clear();
            checkpoint.push_back(-1);
            return false;
        default:
            return false;
        }
    }
};


