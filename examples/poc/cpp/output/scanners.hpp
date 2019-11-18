#pragma once
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#define SCAN_ALL \
    Token,\
    Scanner,\
    IdentifierScanner,\
    WhitespaceScanner,\
    NumberScanner,\
    CharacterScanner,\
    StringScanner,\
    DotScanner,\
    LparenScanner,\
    RparenScanner,\
    CommaScanner,\
    StarScanner,\
    EqualScanner,\
    LbraceScanner,\
    RbraceScanner,\
    ColonScanner,\
    LbracketScanner,\
    RbracketScanner,\
    PlusScanner,\
    MinusScanner,\
    SlashScanner,\
    LessthanScanner,\
    GreaterthanScanner

enum class Token {
    empty = 0,
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
    bool accepts;
    int error;

    Scanner(Token token) 
        : token(token)
        , checkpoint({0})
        , accepts(false)
        , error(-1)
    {}

    virtual ~Scanner() {}
    virtual bool next(uint32_t) = 0;

    int state() const
    {
        return checkpoint.empty() ? error : checkpoint.back();
    }

    int change_state(int next_state, bool checkpoint=false)
    {
        if (checkpoint) {
            this->checkpoint.clear();
            accepts = true;
        }
        this->checkpoint.push_back(next_state);
        return next_state != error;
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
    IdentifierScanner() : Scanner(Token::identifier)
    {
        error = 2;
    }

    bool next(uint32_t c)
    {
        switch (state()) {
        case 0:
            if (0 <= c && c < 48)
                return change_state(2);
            if (48 <= c && c < 58)
                return change_state(2);
            if (58 <= c && c < 65)
                return change_state(2);
            if (65 <= c && c < 91)
                return change_state(1, true);
            if (91 <= c && c < 95)
                return change_state(2);
            if (95 <= c && c < 96)
                return change_state(1, true);
            if (96 <= c && c < 97)
                return change_state(2);
            if (97 <= c && c < 123)
                return change_state(1, true);
            if (123 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        case 2:
            if (0 <= c && c < 48)
                return change_state(2);
            if (48 <= c && c < 58)
                return change_state(2);
            if (58 <= c && c < 65)
                return change_state(2);
            if (65 <= c && c < 91)
                return change_state(2);
            if (91 <= c && c < 95)
                return change_state(2);
            if (95 <= c && c < 96)
                return change_state(2);
            if (96 <= c && c < 97)
                return change_state(2);
            if (97 <= c && c < 123)
                return change_state(2);
            if (123 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        case 1:
            if (0 <= c && c < 48)
                return change_state(2);
            if (48 <= c && c < 58)
                return change_state(1, true);
            if (58 <= c && c < 65)
                return change_state(2);
            if (65 <= c && c < 91)
                return change_state(1, true);
            if (91 <= c && c < 95)
                return change_state(2);
            if (95 <= c && c < 96)
                return change_state(1, true);
            if (96 <= c && c < 97)
                return change_state(2);
            if (97 <= c && c < 123)
                return change_state(1, true);
            if (123 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        default:
            return change_state(error);
        }
    }
};

struct WhitespaceScanner: public Scanner {
    WhitespaceScanner() : Scanner(Token::whitespace)
    {
        error = 2;
    }

    bool next(uint32_t c)
    {
        switch (state()) {
        case 0:
            if (0 <= c && c < 9)
                return change_state(2);
            if (9 <= c && c < 10)
                return change_state(1, true);
            if (10 <= c && c < 11)
                return change_state(1, true);
            if (11 <= c && c < 32)
                return change_state(2);
            if (32 <= c && c < 33)
                return change_state(1, true);
            if (33 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        case 2:
            if (0 <= c && c < 9)
                return change_state(2);
            if (9 <= c && c < 10)
                return change_state(2);
            if (10 <= c && c < 11)
                return change_state(2);
            if (11 <= c && c < 32)
                return change_state(2);
            if (32 <= c && c < 33)
                return change_state(2);
            if (33 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        case 1:
            if (0 <= c && c < 9)
                return change_state(2);
            if (9 <= c && c < 10)
                return change_state(2);
            if (10 <= c && c < 11)
                return change_state(2);
            if (11 <= c && c < 32)
                return change_state(2);
            if (32 <= c && c < 33)
                return change_state(2);
            if (33 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        default:
            return change_state(error);
        }
    }
};

struct NumberScanner: public Scanner {
    NumberScanner() : Scanner(Token::number)
    {
        error = 6;
    }

    bool next(uint32_t c)
    {
        switch (state()) {
        case 0:
            if (0 <= c && c < 43)
                return change_state(6);
            if (43 <= c && c < 44)
                return change_state(6);
            if (44 <= c && c < 45)
                return change_state(6);
            if (45 <= c && c < 46)
                return change_state(6);
            if (46 <= c && c < 47)
                return change_state(6);
            if (47 <= c && c < 48)
                return change_state(6);
            if (48 <= c && c < 49)
                return change_state(3, true);
            if (49 <= c && c < 58)
                return change_state(8, true);
            if (58 <= c && c < 69)
                return change_state(6);
            if (69 <= c && c < 70)
                return change_state(6);
            if (70 <= c && c < 101)
                return change_state(6);
            if (101 <= c && c < 102)
                return change_state(6);
            if (102 <= c && c < 4294967295)
                return change_state(6);
            return change_state(error);
        case 6:
            if (0 <= c && c < 43)
                return change_state(6);
            if (43 <= c && c < 44)
                return change_state(6);
            if (44 <= c && c < 45)
                return change_state(6);
            if (45 <= c && c < 46)
                return change_state(6);
            if (46 <= c && c < 47)
                return change_state(6);
            if (47 <= c && c < 48)
                return change_state(6);
            if (48 <= c && c < 49)
                return change_state(6);
            if (49 <= c && c < 58)
                return change_state(6);
            if (58 <= c && c < 69)
                return change_state(6);
            if (69 <= c && c < 70)
                return change_state(6);
            if (70 <= c && c < 101)
                return change_state(6);
            if (101 <= c && c < 102)
                return change_state(6);
            if (102 <= c && c < 4294967295)
                return change_state(6);
            return change_state(error);
        case 3:
            if (0 <= c && c < 43)
                return change_state(6);
            if (43 <= c && c < 44)
                return change_state(6);
            if (44 <= c && c < 45)
                return change_state(6);
            if (45 <= c && c < 46)
                return change_state(6);
            if (46 <= c && c < 47)
                return change_state(7);
            if (47 <= c && c < 48)
                return change_state(6);
            if (48 <= c && c < 49)
                return change_state(6);
            if (49 <= c && c < 58)
                return change_state(6);
            if (58 <= c && c < 69)
                return change_state(6);
            if (69 <= c && c < 70)
                return change_state(5);
            if (70 <= c && c < 101)
                return change_state(6);
            if (101 <= c && c < 102)
                return change_state(5);
            if (102 <= c && c < 4294967295)
                return change_state(6);
            return change_state(error);
        case 8:
            if (0 <= c && c < 43)
                return change_state(6);
            if (43 <= c && c < 44)
                return change_state(6);
            if (44 <= c && c < 45)
                return change_state(6);
            if (45 <= c && c < 46)
                return change_state(6);
            if (46 <= c && c < 47)
                return change_state(7);
            if (47 <= c && c < 48)
                return change_state(6);
            if (48 <= c && c < 49)
                return change_state(8, true);
            if (49 <= c && c < 58)
                return change_state(8, true);
            if (58 <= c && c < 69)
                return change_state(6);
            if (69 <= c && c < 70)
                return change_state(5);
            if (70 <= c && c < 101)
                return change_state(6);
            if (101 <= c && c < 102)
                return change_state(5);
            if (102 <= c && c < 4294967295)
                return change_state(6);
            return change_state(error);
        case 1:
            if (0 <= c && c < 43)
                return change_state(6);
            if (43 <= c && c < 44)
                return change_state(6);
            if (44 <= c && c < 45)
                return change_state(6);
            if (45 <= c && c < 46)
                return change_state(6);
            if (46 <= c && c < 47)
                return change_state(6);
            if (47 <= c && c < 48)
                return change_state(6);
            if (48 <= c && c < 49)
                return change_state(6);
            if (49 <= c && c < 58)
                return change_state(6);
            if (58 <= c && c < 69)
                return change_state(6);
            if (69 <= c && c < 70)
                return change_state(6);
            if (70 <= c && c < 101)
                return change_state(6);
            if (101 <= c && c < 102)
                return change_state(6);
            if (102 <= c && c < 4294967295)
                return change_state(6);
            return change_state(error);
        case 2:
            if (0 <= c && c < 43)
                return change_state(6);
            if (43 <= c && c < 44)
                return change_state(6);
            if (44 <= c && c < 45)
                return change_state(6);
            if (45 <= c && c < 46)
                return change_state(6);
            if (46 <= c && c < 47)
                return change_state(6);
            if (47 <= c && c < 48)
                return change_state(6);
            if (48 <= c && c < 49)
                return change_state(1, true);
            if (49 <= c && c < 58)
                return change_state(4, true);
            if (58 <= c && c < 69)
                return change_state(6);
            if (69 <= c && c < 70)
                return change_state(6);
            if (70 <= c && c < 101)
                return change_state(6);
            if (101 <= c && c < 102)
                return change_state(6);
            if (102 <= c && c < 4294967295)
                return change_state(6);
            return change_state(error);
        case 4:
            if (0 <= c && c < 43)
                return change_state(6);
            if (43 <= c && c < 44)
                return change_state(6);
            if (44 <= c && c < 45)
                return change_state(6);
            if (45 <= c && c < 46)
                return change_state(6);
            if (46 <= c && c < 47)
                return change_state(6);
            if (47 <= c && c < 48)
                return change_state(6);
            if (48 <= c && c < 49)
                return change_state(4, true);
            if (49 <= c && c < 58)
                return change_state(4, true);
            if (58 <= c && c < 69)
                return change_state(6);
            if (69 <= c && c < 70)
                return change_state(6);
            if (70 <= c && c < 101)
                return change_state(6);
            if (101 <= c && c < 102)
                return change_state(6);
            if (102 <= c && c < 4294967295)
                return change_state(6);
            return change_state(error);
        case 7:
            if (0 <= c && c < 43)
                return change_state(6);
            if (43 <= c && c < 44)
                return change_state(6);
            if (44 <= c && c < 45)
                return change_state(6);
            if (45 <= c && c < 46)
                return change_state(6);
            if (46 <= c && c < 47)
                return change_state(6);
            if (47 <= c && c < 48)
                return change_state(6);
            if (48 <= c && c < 49)
                return change_state(9, true);
            if (49 <= c && c < 58)
                return change_state(9, true);
            if (58 <= c && c < 69)
                return change_state(6);
            if (69 <= c && c < 70)
                return change_state(6);
            if (70 <= c && c < 101)
                return change_state(6);
            if (101 <= c && c < 102)
                return change_state(6);
            if (102 <= c && c < 4294967295)
                return change_state(6);
            return change_state(error);
        case 5:
            if (0 <= c && c < 43)
                return change_state(6);
            if (43 <= c && c < 44)
                return change_state(2);
            if (44 <= c && c < 45)
                return change_state(6);
            if (45 <= c && c < 46)
                return change_state(2);
            if (46 <= c && c < 47)
                return change_state(6);
            if (47 <= c && c < 48)
                return change_state(6);
            if (48 <= c && c < 49)
                return change_state(1, true);
            if (49 <= c && c < 58)
                return change_state(4, true);
            if (58 <= c && c < 69)
                return change_state(6);
            if (69 <= c && c < 70)
                return change_state(6);
            if (70 <= c && c < 101)
                return change_state(6);
            if (101 <= c && c < 102)
                return change_state(6);
            if (102 <= c && c < 4294967295)
                return change_state(6);
            return change_state(error);
        case 9:
            if (0 <= c && c < 43)
                return change_state(6);
            if (43 <= c && c < 44)
                return change_state(6);
            if (44 <= c && c < 45)
                return change_state(6);
            if (45 <= c && c < 46)
                return change_state(6);
            if (46 <= c && c < 47)
                return change_state(6);
            if (47 <= c && c < 48)
                return change_state(6);
            if (48 <= c && c < 49)
                return change_state(9, true);
            if (49 <= c && c < 58)
                return change_state(9, true);
            if (58 <= c && c < 69)
                return change_state(6);
            if (69 <= c && c < 70)
                return change_state(5);
            if (70 <= c && c < 101)
                return change_state(6);
            if (101 <= c && c < 102)
                return change_state(5);
            if (102 <= c && c < 4294967295)
                return change_state(6);
            return change_state(error);
        default:
            return change_state(error);
        }
    }
};

struct CharacterScanner: public Scanner {
    CharacterScanner() : Scanner(Token::character)
    {
        error = 5;
    }

    bool next(uint32_t c)
    {
        switch (state()) {
        case 0:
            if (0 <= c && c < 32)
                return change_state(5);
            if (32 <= c && c < 39)
                return change_state(5);
            if (39 <= c && c < 40)
                return change_state(3);
            if (40 <= c && c < 92)
                return change_state(5);
            if (92 <= c && c < 93)
                return change_state(5);
            if (93 <= c && c < 110)
                return change_state(5);
            if (110 <= c && c < 111)
                return change_state(5);
            if (111 <= c && c < 116)
                return change_state(5);
            if (116 <= c && c < 117)
                return change_state(5);
            if (117 <= c && c < 127)
                return change_state(5);
            if (127 <= c && c < 4294967295)
                return change_state(5);
            return change_state(error);
        case 5:
            if (0 <= c && c < 32)
                return change_state(5);
            if (32 <= c && c < 39)
                return change_state(5);
            if (39 <= c && c < 40)
                return change_state(5);
            if (40 <= c && c < 92)
                return change_state(5);
            if (92 <= c && c < 93)
                return change_state(5);
            if (93 <= c && c < 110)
                return change_state(5);
            if (110 <= c && c < 111)
                return change_state(5);
            if (111 <= c && c < 116)
                return change_state(5);
            if (116 <= c && c < 117)
                return change_state(5);
            if (117 <= c && c < 127)
                return change_state(5);
            if (127 <= c && c < 4294967295)
                return change_state(5);
            return change_state(error);
        case 3:
            if (0 <= c && c < 32)
                return change_state(5);
            if (32 <= c && c < 39)
                return change_state(2);
            if (39 <= c && c < 40)
                return change_state(5);
            if (40 <= c && c < 92)
                return change_state(2);
            if (92 <= c && c < 93)
                return change_state(4);
            if (93 <= c && c < 110)
                return change_state(2);
            if (110 <= c && c < 111)
                return change_state(2);
            if (111 <= c && c < 116)
                return change_state(2);
            if (116 <= c && c < 117)
                return change_state(2);
            if (117 <= c && c < 127)
                return change_state(2);
            if (127 <= c && c < 4294967295)
                return change_state(5);
            return change_state(error);
        case 1:
            if (0 <= c && c < 32)
                return change_state(5);
            if (32 <= c && c < 39)
                return change_state(5);
            if (39 <= c && c < 40)
                return change_state(5);
            if (40 <= c && c < 92)
                return change_state(5);
            if (92 <= c && c < 93)
                return change_state(5);
            if (93 <= c && c < 110)
                return change_state(5);
            if (110 <= c && c < 111)
                return change_state(5);
            if (111 <= c && c < 116)
                return change_state(5);
            if (116 <= c && c < 117)
                return change_state(5);
            if (117 <= c && c < 127)
                return change_state(5);
            if (127 <= c && c < 4294967295)
                return change_state(5);
            return change_state(error);
        case 2:
            if (0 <= c && c < 32)
                return change_state(5);
            if (32 <= c && c < 39)
                return change_state(5);
            if (39 <= c && c < 40)
                return change_state(1, true);
            if (40 <= c && c < 92)
                return change_state(5);
            if (92 <= c && c < 93)
                return change_state(5);
            if (93 <= c && c < 110)
                return change_state(5);
            if (110 <= c && c < 111)
                return change_state(5);
            if (111 <= c && c < 116)
                return change_state(5);
            if (116 <= c && c < 117)
                return change_state(5);
            if (117 <= c && c < 127)
                return change_state(5);
            if (127 <= c && c < 4294967295)
                return change_state(5);
            return change_state(error);
        case 4:
            if (0 <= c && c < 32)
                return change_state(5);
            if (32 <= c && c < 39)
                return change_state(5);
            if (39 <= c && c < 40)
                return change_state(2);
            if (40 <= c && c < 92)
                return change_state(5);
            if (92 <= c && c < 93)
                return change_state(2);
            if (93 <= c && c < 110)
                return change_state(5);
            if (110 <= c && c < 111)
                return change_state(2);
            if (111 <= c && c < 116)
                return change_state(5);
            if (116 <= c && c < 117)
                return change_state(2);
            if (117 <= c && c < 127)
                return change_state(5);
            if (127 <= c && c < 4294967295)
                return change_state(5);
            return change_state(error);
        default:
            return change_state(error);
        }
    }
};

struct StringScanner: public Scanner {
    StringScanner() : Scanner(Token::string)
    {
        error = 4;
    }

    bool next(uint32_t c)
    {
        switch (state()) {
        case 0:
            if (0 <= c && c < 32)
                return change_state(4);
            if (32 <= c && c < 34)
                return change_state(4);
            if (34 <= c && c < 35)
                return change_state(2);
            if (35 <= c && c < 92)
                return change_state(4);
            if (92 <= c && c < 93)
                return change_state(4);
            if (93 <= c && c < 127)
                return change_state(4);
            if (127 <= c && c < 4294967295)
                return change_state(4);
            return change_state(error);
        case 4:
            if (0 <= c && c < 32)
                return change_state(4);
            if (32 <= c && c < 34)
                return change_state(4);
            if (34 <= c && c < 35)
                return change_state(4);
            if (35 <= c && c < 92)
                return change_state(4);
            if (92 <= c && c < 93)
                return change_state(4);
            if (93 <= c && c < 127)
                return change_state(4);
            if (127 <= c && c < 4294967295)
                return change_state(4);
            return change_state(error);
        case 2:
            if (0 <= c && c < 32)
                return change_state(4);
            if (32 <= c && c < 34)
                return change_state(2);
            if (34 <= c && c < 35)
                return change_state(1, true);
            if (35 <= c && c < 92)
                return change_state(2);
            if (92 <= c && c < 93)
                return change_state(3);
            if (93 <= c && c < 127)
                return change_state(2);
            if (127 <= c && c < 4294967295)
                return change_state(4);
            return change_state(error);
        case 1:
            if (0 <= c && c < 32)
                return change_state(4);
            if (32 <= c && c < 34)
                return change_state(4);
            if (34 <= c && c < 35)
                return change_state(4);
            if (35 <= c && c < 92)
                return change_state(4);
            if (92 <= c && c < 93)
                return change_state(4);
            if (93 <= c && c < 127)
                return change_state(4);
            if (127 <= c && c < 4294967295)
                return change_state(4);
            return change_state(error);
        case 3:
            if (0 <= c && c < 32)
                return change_state(4);
            if (32 <= c && c < 34)
                return change_state(2);
            if (34 <= c && c < 35)
                return change_state(2);
            if (35 <= c && c < 92)
                return change_state(2);
            if (92 <= c && c < 93)
                return change_state(2);
            if (93 <= c && c < 127)
                return change_state(2);
            if (127 <= c && c < 4294967295)
                return change_state(4);
            return change_state(error);
        default:
            return change_state(error);
        }
    }
};

struct DotScanner: public Scanner {
    DotScanner() : Scanner(Token::dot)
    {
        error = 2;
    }

    bool next(uint32_t c)
    {
        switch (state()) {
        case 0:
            if (0 <= c && c < 46)
                return change_state(2);
            if (46 <= c && c < 47)
                return change_state(1, true);
            if (47 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        case 2:
            if (0 <= c && c < 46)
                return change_state(2);
            if (46 <= c && c < 47)
                return change_state(2);
            if (47 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        case 1:
            if (0 <= c && c < 46)
                return change_state(2);
            if (46 <= c && c < 47)
                return change_state(2);
            if (47 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        default:
            return change_state(error);
        }
    }
};

struct LparenScanner: public Scanner {
    LparenScanner() : Scanner(Token::lparen)
    {
        error = 2;
    }

    bool next(uint32_t c)
    {
        switch (state()) {
        case 0:
            if (0 <= c && c < 40)
                return change_state(2);
            if (40 <= c && c < 41)
                return change_state(1, true);
            if (41 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        case 2:
            if (0 <= c && c < 40)
                return change_state(2);
            if (40 <= c && c < 41)
                return change_state(2);
            if (41 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        case 1:
            if (0 <= c && c < 40)
                return change_state(2);
            if (40 <= c && c < 41)
                return change_state(2);
            if (41 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        default:
            return change_state(error);
        }
    }
};

struct RparenScanner: public Scanner {
    RparenScanner() : Scanner(Token::rparen)
    {
        error = 2;
    }

    bool next(uint32_t c)
    {
        switch (state()) {
        case 0:
            if (0 <= c && c < 41)
                return change_state(2);
            if (41 <= c && c < 42)
                return change_state(1, true);
            if (42 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        case 2:
            if (0 <= c && c < 41)
                return change_state(2);
            if (41 <= c && c < 42)
                return change_state(2);
            if (42 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        case 1:
            if (0 <= c && c < 41)
                return change_state(2);
            if (41 <= c && c < 42)
                return change_state(2);
            if (42 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        default:
            return change_state(error);
        }
    }
};

struct CommaScanner: public Scanner {
    CommaScanner() : Scanner(Token::comma)
    {
        error = 2;
    }

    bool next(uint32_t c)
    {
        switch (state()) {
        case 0:
            if (0 <= c && c < 44)
                return change_state(2);
            if (44 <= c && c < 45)
                return change_state(1, true);
            if (45 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        case 2:
            if (0 <= c && c < 44)
                return change_state(2);
            if (44 <= c && c < 45)
                return change_state(2);
            if (45 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        case 1:
            if (0 <= c && c < 44)
                return change_state(2);
            if (44 <= c && c < 45)
                return change_state(2);
            if (45 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        default:
            return change_state(error);
        }
    }
};

struct StarScanner: public Scanner {
    StarScanner() : Scanner(Token::star)
    {
        error = 2;
    }

    bool next(uint32_t c)
    {
        switch (state()) {
        case 0:
            if (0 <= c && c < 42)
                return change_state(2);
            if (42 <= c && c < 43)
                return change_state(1, true);
            if (43 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        case 2:
            if (0 <= c && c < 42)
                return change_state(2);
            if (42 <= c && c < 43)
                return change_state(2);
            if (43 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        case 1:
            if (0 <= c && c < 42)
                return change_state(2);
            if (42 <= c && c < 43)
                return change_state(2);
            if (43 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        default:
            return change_state(error);
        }
    }
};

struct EqualScanner: public Scanner {
    EqualScanner() : Scanner(Token::equal)
    {
        error = 2;
    }

    bool next(uint32_t c)
    {
        switch (state()) {
        case 0:
            if (0 <= c && c < 61)
                return change_state(2);
            if (61 <= c && c < 62)
                return change_state(1, true);
            if (62 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        case 2:
            if (0 <= c && c < 61)
                return change_state(2);
            if (61 <= c && c < 62)
                return change_state(2);
            if (62 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        case 1:
            if (0 <= c && c < 61)
                return change_state(2);
            if (61 <= c && c < 62)
                return change_state(2);
            if (62 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        default:
            return change_state(error);
        }
    }
};

struct LbraceScanner: public Scanner {
    LbraceScanner() : Scanner(Token::lbrace)
    {
        error = 2;
    }

    bool next(uint32_t c)
    {
        switch (state()) {
        case 0:
            if (0 <= c && c < 123)
                return change_state(2);
            if (123 <= c && c < 124)
                return change_state(1, true);
            if (124 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        case 2:
            if (0 <= c && c < 123)
                return change_state(2);
            if (123 <= c && c < 124)
                return change_state(2);
            if (124 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        case 1:
            if (0 <= c && c < 123)
                return change_state(2);
            if (123 <= c && c < 124)
                return change_state(2);
            if (124 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        default:
            return change_state(error);
        }
    }
};

struct RbraceScanner: public Scanner {
    RbraceScanner() : Scanner(Token::rbrace)
    {
        error = 2;
    }

    bool next(uint32_t c)
    {
        switch (state()) {
        case 0:
            if (0 <= c && c < 125)
                return change_state(2);
            if (125 <= c && c < 126)
                return change_state(1, true);
            if (126 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        case 2:
            if (0 <= c && c < 125)
                return change_state(2);
            if (125 <= c && c < 126)
                return change_state(2);
            if (126 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        case 1:
            if (0 <= c && c < 125)
                return change_state(2);
            if (125 <= c && c < 126)
                return change_state(2);
            if (126 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        default:
            return change_state(error);
        }
    }
};

struct ColonScanner: public Scanner {
    ColonScanner() : Scanner(Token::colon)
    {
        error = 2;
    }

    bool next(uint32_t c)
    {
        switch (state()) {
        case 0:
            if (0 <= c && c < 58)
                return change_state(2);
            if (58 <= c && c < 59)
                return change_state(1, true);
            if (59 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        case 2:
            if (0 <= c && c < 58)
                return change_state(2);
            if (58 <= c && c < 59)
                return change_state(2);
            if (59 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        case 1:
            if (0 <= c && c < 58)
                return change_state(2);
            if (58 <= c && c < 59)
                return change_state(2);
            if (59 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        default:
            return change_state(error);
        }
    }
};

struct LbracketScanner: public Scanner {
    LbracketScanner() : Scanner(Token::lbracket)
    {
        error = 2;
    }

    bool next(uint32_t c)
    {
        switch (state()) {
        case 0:
            if (0 <= c && c < 91)
                return change_state(2);
            if (91 <= c && c < 92)
                return change_state(1, true);
            if (92 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        case 2:
            if (0 <= c && c < 91)
                return change_state(2);
            if (91 <= c && c < 92)
                return change_state(2);
            if (92 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        case 1:
            if (0 <= c && c < 91)
                return change_state(2);
            if (91 <= c && c < 92)
                return change_state(2);
            if (92 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        default:
            return change_state(error);
        }
    }
};

struct RbracketScanner: public Scanner {
    RbracketScanner() : Scanner(Token::rbracket)
    {
        error = 2;
    }

    bool next(uint32_t c)
    {
        switch (state()) {
        case 0:
            if (0 <= c && c < 93)
                return change_state(2);
            if (93 <= c && c < 94)
                return change_state(1, true);
            if (94 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        case 2:
            if (0 <= c && c < 93)
                return change_state(2);
            if (93 <= c && c < 94)
                return change_state(2);
            if (94 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        case 1:
            if (0 <= c && c < 93)
                return change_state(2);
            if (93 <= c && c < 94)
                return change_state(2);
            if (94 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        default:
            return change_state(error);
        }
    }
};

struct PlusScanner: public Scanner {
    PlusScanner() : Scanner(Token::plus)
    {
        error = 2;
    }

    bool next(uint32_t c)
    {
        switch (state()) {
        case 0:
            if (0 <= c && c < 43)
                return change_state(2);
            if (43 <= c && c < 44)
                return change_state(1, true);
            if (44 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        case 2:
            if (0 <= c && c < 43)
                return change_state(2);
            if (43 <= c && c < 44)
                return change_state(2);
            if (44 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        case 1:
            if (0 <= c && c < 43)
                return change_state(2);
            if (43 <= c && c < 44)
                return change_state(2);
            if (44 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        default:
            return change_state(error);
        }
    }
};

struct MinusScanner: public Scanner {
    MinusScanner() : Scanner(Token::minus)
    {
        error = 2;
    }

    bool next(uint32_t c)
    {
        switch (state()) {
        case 0:
            if (0 <= c && c < 45)
                return change_state(2);
            if (45 <= c && c < 46)
                return change_state(1, true);
            if (46 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        case 2:
            if (0 <= c && c < 45)
                return change_state(2);
            if (45 <= c && c < 46)
                return change_state(2);
            if (46 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        case 1:
            if (0 <= c && c < 45)
                return change_state(2);
            if (45 <= c && c < 46)
                return change_state(2);
            if (46 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        default:
            return change_state(error);
        }
    }
};

struct SlashScanner: public Scanner {
    SlashScanner() : Scanner(Token::slash)
    {
        error = 2;
    }

    bool next(uint32_t c)
    {
        switch (state()) {
        case 0:
            if (0 <= c && c < 47)
                return change_state(2);
            if (47 <= c && c < 48)
                return change_state(1, true);
            if (48 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        case 2:
            if (0 <= c && c < 47)
                return change_state(2);
            if (47 <= c && c < 48)
                return change_state(2);
            if (48 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        case 1:
            if (0 <= c && c < 47)
                return change_state(2);
            if (47 <= c && c < 48)
                return change_state(2);
            if (48 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        default:
            return change_state(error);
        }
    }
};

struct LessthanScanner: public Scanner {
    LessthanScanner() : Scanner(Token::lessthan)
    {
        error = 2;
    }

    bool next(uint32_t c)
    {
        switch (state()) {
        case 0:
            if (0 <= c && c < 60)
                return change_state(2);
            if (60 <= c && c < 61)
                return change_state(1, true);
            if (61 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        case 2:
            if (0 <= c && c < 60)
                return change_state(2);
            if (60 <= c && c < 61)
                return change_state(2);
            if (61 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        case 1:
            if (0 <= c && c < 60)
                return change_state(2);
            if (60 <= c && c < 61)
                return change_state(2);
            if (61 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        default:
            return change_state(error);
        }
    }
};

struct GreaterthanScanner: public Scanner {
    GreaterthanScanner() : Scanner(Token::greaterthan)
    {
        error = 2;
    }

    bool next(uint32_t c)
    {
        switch (state()) {
        case 0:
            if (0 <= c && c < 62)
                return change_state(2);
            if (62 <= c && c < 63)
                return change_state(1, true);
            if (63 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        case 2:
            if (0 <= c && c < 62)
                return change_state(2);
            if (62 <= c && c < 63)
                return change_state(2);
            if (63 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        case 1:
            if (0 <= c && c < 62)
                return change_state(2);
            if (62 <= c && c < 63)
                return change_state(2);
            if (63 <= c && c < 4294967295)
                return change_state(2);
            return change_state(error);
        default:
            return change_state(error);
        }
    }
};


