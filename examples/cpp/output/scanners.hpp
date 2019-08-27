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
    bool accept;

    Scanner(Token token) : token(token), checkpoint({0}), accept(false) {}
    virtual ~Scanner() {}
    virtual bool next(int) = 0;

    int state() const
    {
        return checkpoint.empty() ? -1 : checkpoint.back();
    }

    int change_state(int next_state, bool checkpoint=false)
    {
        if (checkpoint) {
            this->checkpoint.clear();
            accept = true;
        }
        this->checkpoint.push_back(next_state);
        return next_state != -1;
    }

    int backtrack_steps() const
    {
        return checkpoint.size() - 1;
    }
};

std::ostream& operator<<(std::ostream& out, const Scanner& scanner)
{
    return out << "<Scanner " << scanner.token << " state:" << scanner.state()
        << " checkpoint:[";
    for (const auto& state: scanner.checkpoint) {
        out << " " << state;
    }
    return out << " ]>";
}

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
                return change_state(1, true);
            }
            if (c == 95) {
                return change_state(1, true);
            }
            if (97 <= c && c <= 122) {
                return change_state(1, true);
            }
            return change_state(-1);
        case 1:
            accept = state();
            checkpoint.clear();
            if (48 <= c && c <= 57) {
                return change_state(1, true);
            }
            if (65 <= c && c <= 90) {
                return change_state(1, true);
            }
            if (c == 95) {
                return change_state(1, true);
            }
            if (97 <= c && c <= 122) {
                return change_state(1, true);
            }
            return change_state(-1);
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
            if (c == 9) {
                return change_state(1, true);
            }
            if (c == 10) {
                return change_state(1, true);
            }
            if (c == 32) {
                return change_state(1, true);
            }
            return change_state(-1);
        case 1:
            accept = state();
            checkpoint.clear();
            return change_state(-1);
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
            if (c == 48) {
                return change_state(5, true);
            }
            if (49 <= c && c <= 57) {
                return change_state(1, true);
            }
            return change_state(-1);
        case 5:
            accept = state();
            checkpoint.clear();
            if (c == 46) {
                return change_state(7);
            }
            if (c == 69) {
                return change_state(2);
            }
            if (c == 101) {
                return change_state(2);
            }
            return change_state(-1);
        case 1:
            accept = state();
            checkpoint.clear();
            if (c == 46) {
                return change_state(7);
            }
            if (c == 48) {
                return change_state(1, true);
            }
            if (49 <= c && c <= 57) {
                return change_state(1, true);
            }
            if (c == 69) {
                return change_state(2);
            }
            if (c == 101) {
                return change_state(2);
            }
            return change_state(-1);
        case 7:
            if (c == 48) {
                return change_state(3, true);
            }
            if (49 <= c && c <= 57) {
                return change_state(3, true);
            }
            return change_state(-1);
        case 2:
            if (c == 43) {
                return change_state(4);
            }
            if (c == 45) {
                return change_state(4);
            }
            if (c == 48) {
                return change_state(6, true);
            }
            if (49 <= c && c <= 57) {
                return change_state(8, true);
            }
            return change_state(-1);
        case 4:
            if (c == 48) {
                return change_state(6, true);
            }
            if (49 <= c && c <= 57) {
                return change_state(8, true);
            }
            return change_state(-1);
        case 6:
            accept = state();
            checkpoint.clear();
            return change_state(-1);
        case 8:
            accept = state();
            checkpoint.clear();
            if (c == 48) {
                return change_state(8, true);
            }
            if (49 <= c && c <= 57) {
                return change_state(8, true);
            }
            return change_state(-1);
        case 3:
            accept = state();
            checkpoint.clear();
            if (c == 48) {
                return change_state(3, true);
            }
            if (49 <= c && c <= 57) {
                return change_state(3, true);
            }
            if (c == 69) {
                return change_state(2);
            }
            if (c == 101) {
                return change_state(2);
            }
            return change_state(-1);
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
            if (c == 39) {
                return change_state(2);
            }
            return change_state(-1);
        case 2:
            if (32 <= c && c <= 38) {
                return change_state(3);
            }
            if (40 <= c && c <= 91) {
                return change_state(3);
            }
            if (c == 92) {
                return change_state(4);
            }
            if (93 <= c && c <= 126) {
                return change_state(3);
            }
            return change_state(-1);
        case 1:
            accept = state();
            checkpoint.clear();
            return change_state(-1);
        case 3:
            if (c == 39) {
                return change_state(1, true);
            }
            return change_state(-1);
        case 4:
            if (c == 39) {
                return change_state(3);
            }
            if (c == 92) {
                return change_state(3);
            }
            if (93 <= c && c <= 126) {
                return change_state(3);
            }
            return change_state(-1);
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
            if (c == 34) {
                return change_state(2);
            }
            return change_state(-1);
        case 2:
            if (32 <= c && c <= 33) {
                return change_state(2);
            }
            if (c == 34) {
                return change_state(1, true);
            }
            if (35 <= c && c <= 91) {
                return change_state(2);
            }
            if (c == 92) {
                return change_state(3);
            }
            if (93 <= c && c <= 126) {
                return change_state(2);
            }
            return change_state(-1);
        case 1:
            accept = state();
            checkpoint.clear();
            return change_state(-1);
        case 3:
            if (32 <= c && c <= 33) {
                return change_state(2);
            }
            if (c == 34) {
                return change_state(2);
            }
            if (35 <= c && c <= 91) {
                return change_state(2);
            }
            if (c == 92) {
                return change_state(2);
            }
            if (93 <= c && c <= 126) {
                return change_state(2);
            }
            return change_state(-1);
        default:
            return false;
        }
    }
};

struct DotScanner: public Scanner {
    //using Scanner::Scanner;
    DotScanner() : Scanner(Token::dot) {}

    bool next(int c)
    {
        int eof = std::char_traits<char>::eof();
        if (c == eof && state() != -1) {
            checkpoint.push_back(-1);
        }
        switch (state()) {
        case 0:
            if (c == 46) {
                return change_state(1, true);
            }
            return change_state(-1);
        case 1:
            accept = state();
            checkpoint.clear();
            return change_state(-1);
        default:
            return false;
        }
    }
};

struct LparenScanner: public Scanner {
    //using Scanner::Scanner;
    LparenScanner() : Scanner(Token::lparen) {}

    bool next(int c)
    {
        int eof = std::char_traits<char>::eof();
        if (c == eof && state() != -1) {
            checkpoint.push_back(-1);
        }
        switch (state()) {
        case 0:
            if (c == 40) {
                return change_state(1, true);
            }
            return change_state(-1);
        case 1:
            accept = state();
            checkpoint.clear();
            return change_state(-1);
        default:
            return false;
        }
    }
};

struct RparenScanner: public Scanner {
    //using Scanner::Scanner;
    RparenScanner() : Scanner(Token::rparen) {}

    bool next(int c)
    {
        int eof = std::char_traits<char>::eof();
        if (c == eof && state() != -1) {
            checkpoint.push_back(-1);
        }
        switch (state()) {
        case 0:
            if (c == 41) {
                return change_state(1, true);
            }
            return change_state(-1);
        case 1:
            accept = state();
            checkpoint.clear();
            return change_state(-1);
        default:
            return false;
        }
    }
};

struct CommaScanner: public Scanner {
    //using Scanner::Scanner;
    CommaScanner() : Scanner(Token::comma) {}

    bool next(int c)
    {
        int eof = std::char_traits<char>::eof();
        if (c == eof && state() != -1) {
            checkpoint.push_back(-1);
        }
        switch (state()) {
        case 0:
            if (c == 44) {
                return change_state(1, true);
            }
            return change_state(-1);
        case 1:
            accept = state();
            checkpoint.clear();
            return change_state(-1);
        default:
            return false;
        }
    }
};

struct StarScanner: public Scanner {
    //using Scanner::Scanner;
    StarScanner() : Scanner(Token::star) {}

    bool next(int c)
    {
        int eof = std::char_traits<char>::eof();
        if (c == eof && state() != -1) {
            checkpoint.push_back(-1);
        }
        switch (state()) {
        case 0:
            if (c == 42) {
                return change_state(1, true);
            }
            return change_state(-1);
        case 1:
            accept = state();
            checkpoint.clear();
            return change_state(-1);
        default:
            return false;
        }
    }
};

struct EqualScanner: public Scanner {
    //using Scanner::Scanner;
    EqualScanner() : Scanner(Token::equal) {}

    bool next(int c)
    {
        int eof = std::char_traits<char>::eof();
        if (c == eof && state() != -1) {
            checkpoint.push_back(-1);
        }
        switch (state()) {
        case 0:
            if (c == 61) {
                return change_state(1, true);
            }
            return change_state(-1);
        case 1:
            accept = state();
            checkpoint.clear();
            return change_state(-1);
        default:
            return false;
        }
    }
};

struct LbraceScanner: public Scanner {
    //using Scanner::Scanner;
    LbraceScanner() : Scanner(Token::lbrace) {}

    bool next(int c)
    {
        int eof = std::char_traits<char>::eof();
        if (c == eof && state() != -1) {
            checkpoint.push_back(-1);
        }
        switch (state()) {
        case 0:
            if (c == 123) {
                return change_state(1, true);
            }
            return change_state(-1);
        case 1:
            accept = state();
            checkpoint.clear();
            return change_state(-1);
        default:
            return false;
        }
    }
};

struct RbraceScanner: public Scanner {
    //using Scanner::Scanner;
    RbraceScanner() : Scanner(Token::rbrace) {}

    bool next(int c)
    {
        int eof = std::char_traits<char>::eof();
        if (c == eof && state() != -1) {
            checkpoint.push_back(-1);
        }
        switch (state()) {
        case 0:
            if (c == 125) {
                return change_state(1, true);
            }
            return change_state(-1);
        case 1:
            accept = state();
            checkpoint.clear();
            return change_state(-1);
        default:
            return false;
        }
    }
};

struct ColonScanner: public Scanner {
    //using Scanner::Scanner;
    ColonScanner() : Scanner(Token::colon) {}

    bool next(int c)
    {
        int eof = std::char_traits<char>::eof();
        if (c == eof && state() != -1) {
            checkpoint.push_back(-1);
        }
        switch (state()) {
        case 0:
            if (c == 58) {
                return change_state(1, true);
            }
            return change_state(-1);
        case 1:
            accept = state();
            checkpoint.clear();
            return change_state(-1);
        default:
            return false;
        }
    }
};

struct LbracketScanner: public Scanner {
    //using Scanner::Scanner;
    LbracketScanner() : Scanner(Token::lbracket) {}

    bool next(int c)
    {
        int eof = std::char_traits<char>::eof();
        if (c == eof && state() != -1) {
            checkpoint.push_back(-1);
        }
        switch (state()) {
        case 0:
            if (c == 91) {
                return change_state(1, true);
            }
            return change_state(-1);
        case 1:
            accept = state();
            checkpoint.clear();
            return change_state(-1);
        default:
            return false;
        }
    }
};

struct RbracketScanner: public Scanner {
    //using Scanner::Scanner;
    RbracketScanner() : Scanner(Token::rbracket) {}

    bool next(int c)
    {
        int eof = std::char_traits<char>::eof();
        if (c == eof && state() != -1) {
            checkpoint.push_back(-1);
        }
        switch (state()) {
        case 0:
            if (c == 93) {
                return change_state(1, true);
            }
            return change_state(-1);
        case 1:
            accept = state();
            checkpoint.clear();
            return change_state(-1);
        default:
            return false;
        }
    }
};

struct PlusScanner: public Scanner {
    //using Scanner::Scanner;
    PlusScanner() : Scanner(Token::plus) {}

    bool next(int c)
    {
        int eof = std::char_traits<char>::eof();
        if (c == eof && state() != -1) {
            checkpoint.push_back(-1);
        }
        switch (state()) {
        case 0:
            if (c == 43) {
                return change_state(1, true);
            }
            return change_state(-1);
        case 1:
            accept = state();
            checkpoint.clear();
            return change_state(-1);
        default:
            return false;
        }
    }
};

struct MinusScanner: public Scanner {
    //using Scanner::Scanner;
    MinusScanner() : Scanner(Token::minus) {}

    bool next(int c)
    {
        int eof = std::char_traits<char>::eof();
        if (c == eof && state() != -1) {
            checkpoint.push_back(-1);
        }
        switch (state()) {
        case 0:
            if (c == 45) {
                return change_state(1, true);
            }
            return change_state(-1);
        case 1:
            accept = state();
            checkpoint.clear();
            return change_state(-1);
        default:
            return false;
        }
    }
};

struct SlashScanner: public Scanner {
    //using Scanner::Scanner;
    SlashScanner() : Scanner(Token::slash) {}

    bool next(int c)
    {
        int eof = std::char_traits<char>::eof();
        if (c == eof && state() != -1) {
            checkpoint.push_back(-1);
        }
        switch (state()) {
        case 0:
            if (c == 47) {
                return change_state(1, true);
            }
            return change_state(-1);
        case 1:
            accept = state();
            checkpoint.clear();
            return change_state(-1);
        default:
            return false;
        }
    }
};

struct LessthanScanner: public Scanner {
    //using Scanner::Scanner;
    LessthanScanner() : Scanner(Token::lessthan) {}

    bool next(int c)
    {
        int eof = std::char_traits<char>::eof();
        if (c == eof && state() != -1) {
            checkpoint.push_back(-1);
        }
        switch (state()) {
        case 0:
            if (c == 60) {
                return change_state(1, true);
            }
            return change_state(-1);
        case 1:
            accept = state();
            checkpoint.clear();
            return change_state(-1);
        default:
            return false;
        }
    }
};

struct GreaterthanScanner: public Scanner {
    //using Scanner::Scanner;
    GreaterthanScanner() : Scanner(Token::greaterthan) {}

    bool next(int c)
    {
        int eof = std::char_traits<char>::eof();
        if (c == eof && state() != -1) {
            checkpoint.push_back(-1);
        }
        switch (state()) {
        case 0:
            if (c == 62) {
                return change_state(1, true);
            }
            return change_state(-1);
        case 1:
            accept = state();
            checkpoint.clear();
            return change_state(-1);
        default:
            return false;
        }
    }
};


