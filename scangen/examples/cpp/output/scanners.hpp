#pragma once
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#define ALL_RECOGNIZERS \
    Identifier, \
    Whitespace, \
    Number, \
    Character, \
    String, \
    Dot, \
    Lparen, \
    Rparen, \
    Comma, \
    Star, \
    Equal, \
    Lbrace, \
    Rbrace, \
    Colon, \
    Lbracket, \
    Rbracket, \
    Plus, \
    Minus, \
    Slash, \
    Lessthan, \
    Greaterthan

enum class Token {
    EMPTY = 0,
    IDENTIFIER,
    WHITESPACE,
    NUMBER,
    CHARACTER,
    STRING,
    DOT,
    LPAREN,
    RPAREN,
    COMMA,
    STAR,
    EQUAL,
    LBRACE,
    RBRACE,
    COLON,
    LBRACKET,
    RBRACKET,
    PLUS,
    MINUS,
    SLASH,
    LESSTHAN,
    GREATERTHAN,
};

std::ostream& operator<<(std::ostream& out, Token token)
{
    switch (token) {
    case Token::EMPTY:
        return out << "empty";
    case Token::IDENTIFIER:
        return out << "identifier";
    case Token::WHITESPACE:
        return out << "whitespace";
    case Token::NUMBER:
        return out << "number";
    case Token::CHARACTER:
        return out << "character";
    case Token::STRING:
        return out << "string";
    case Token::DOT:
        return out << "dot";
    case Token::LPAREN:
        return out << "lparen";
    case Token::RPAREN:
        return out << "rparen";
    case Token::COMMA:
        return out << "comma";
    case Token::STAR:
        return out << "star";
    case Token::EQUAL:
        return out << "equal";
    case Token::LBRACE:
        return out << "lbrace";
    case Token::RBRACE:
        return out << "rbrace";
    case Token::COLON:
        return out << "colon";
    case Token::LBRACKET:
        return out << "lbracket";
    case Token::RBRACKET:
        return out << "rbracket";
    case Token::PLUS:
        return out << "plus";
    case Token::MINUS:
        return out << "minus";
    case Token::SLASH:
        return out << "slash";
    case Token::LESSTHAN:
        return out << "lessthan";
    case Token::GREATERTHAN:
        return out << "greaterthan";
    default:
        return out;
    }
}

struct InputStack {
    std::istream* in;
    std::vector<uint32_t> stack;
    bool done;

    InputStack(std::istream& in = std::cin)
        : in(&in)
        , done(false)
    {}

    uint32_t get()
    {
        if (!stack.empty()) {
            uint32_t a = stack.back();
            stack.pop_back();
            return a;
        }
        uint32_t eof = std::char_traits<char>::eof();
        if (done) {
            return eof;
        }
        uint32_t a = in->get();
        if (a == eof) {
            done = true;
        }
        return a;
    }

    void unget(uint32_t a)
    {
        uint32_t eof = std::char_traits<char>::eof();
        if (a == eof) {
            throw 0;
        }
        stack.push_back(a);
    }
};

struct BaseRecognizer {
    Token token;

    BaseRecognizer(
        Token token = Token::EMPTY,
        bool accept = false, // should be true if 0 is an accept state
        int error = -1)
        : token(token)
        , accept(accept)
        , error(error)
    {}

    virtual ~BaseRecognizer() {}

    virtual std::pair<int, int> next(int q, uint32_t a) const = 0;

    std::pair<bool, std::string> match(InputStack& in)
    {
        std::vector<int> checkpoint = {0};
        std::vector<uint32_t> lexeme;
        uint32_t a = -1;
        uint32_t eof = std::char_traits<char>::eof();
        while (checkpoint.back() != error && (a = in.get()) != eof) {
            auto [status, r] = next(checkpoint.back(), a);
            if (status == 1) {
                accept = true;
                checkpoint.clear();
            }
            checkpoint.push_back(r);
            lexeme.push_back(a);
        }
        for (auto i = checkpoint.size(); i > 1; --i) {
            // rollback to most recent accept state, if any
            // (if accept is true, checkpoint[0] is the most recent accept state)
            uint32_t a = lexeme.back();
            lexeme.pop_back();
            in.unget(a);
        }
        return { accept, std::string(lexeme.begin(), lexeme.end()) };
    }

    std::pair<bool, std::string> match(std::istream& file = std::cin)
    {
        InputStack in(file);
        return match(in);
    }

protected:
    bool accept;
    int const error;
};

struct Identifier: public BaseRecognizer {
    Identifier() : BaseRecognizer(Token::IDENTIFIER, false, 2) {}

    std::pair<int, int> next(int q, uint32_t a) const
    {
        switch (q) {
        case 0:
            if (65 <= a && a < 91)
                return {1, 1};
            if (a == 95)
                return {1, 1};
            if (97 <= a && a < 123)
                return {1, 1};
            return {-1, 2};
        case 1:
            if (48 <= a && a < 58)
                return {1, 1};
            if (65 <= a && a < 91)
                return {1, 1};
            if (a == 95)
                return {1, 1};
            if (97 <= a && a < 123)
                return {1, 1};
            return {-1, 2};
        default:
            return {-1, 2};
        }
    }
};

struct Whitespace: public BaseRecognizer {
    Whitespace() : BaseRecognizer(Token::WHITESPACE, false, 2) {}

    std::pair<int, int> next(int q, uint32_t a) const
    {
        switch (q) {
        case 0:
            if (a == 9)
                return {1, 1};
            if (a == 10)
                return {1, 1};
            if (a == 32)
                return {1, 1};
            return {-1, 2};
        case 1:
            return {-1, 2};
        default:
            return {-1, 2};
        }
    }
};

struct Number: public BaseRecognizer {
    Number() : BaseRecognizer(Token::NUMBER, false, 6) {}

    std::pair<int, int> next(int q, uint32_t a) const
    {
        switch (q) {
        case 0:
            if (a == 48)
                return {1, 3};
            if (49 <= a && a < 58)
                return {1, 8};
            return {-1, 6};
        case 3:
            if (a == 46)
                return {0, 7};
            if (a == 69)
                return {0, 5};
            if (a == 101)
                return {0, 5};
            return {-1, 6};
        case 8:
            if (a == 46)
                return {0, 7};
            if (a == 48)
                return {1, 8};
            if (49 <= a && a < 58)
                return {1, 8};
            if (a == 69)
                return {0, 5};
            if (a == 101)
                return {0, 5};
            return {-1, 6};
        case 1:
            return {-1, 6};
        case 2:
            if (a == 48)
                return {1, 1};
            if (49 <= a && a < 58)
                return {1, 4};
            return {-1, 6};
        case 4:
            if (a == 48)
                return {1, 4};
            if (49 <= a && a < 58)
                return {1, 4};
            return {-1, 6};
        case 7:
            if (a == 48)
                return {1, 9};
            if (49 <= a && a < 58)
                return {1, 9};
            return {-1, 6};
        case 5:
            if (a == 43)
                return {0, 2};
            if (a == 45)
                return {0, 2};
            if (a == 48)
                return {1, 1};
            if (49 <= a && a < 58)
                return {1, 4};
            return {-1, 6};
        case 9:
            if (a == 48)
                return {1, 9};
            if (49 <= a && a < 58)
                return {1, 9};
            if (a == 69)
                return {0, 5};
            if (a == 101)
                return {0, 5};
            return {-1, 6};
        default:
            return {-1, 6};
        }
    }
};

struct Character: public BaseRecognizer {
    Character() : BaseRecognizer(Token::CHARACTER, false, 5) {}

    std::pair<int, int> next(int q, uint32_t a) const
    {
        switch (q) {
        case 0:
            if (a == 39)
                return {0, 3};
            return {-1, 5};
        case 3:
            if (32 <= a && a < 39)
                return {0, 2};
            if (40 <= a && a < 92)
                return {0, 2};
            if (a == 92)
                return {0, 4};
            if (93 <= a && a < 110)
                return {0, 2};
            if (a == 110)
                return {0, 2};
            if (111 <= a && a < 116)
                return {0, 2};
            if (a == 116)
                return {0, 2};
            if (117 <= a && a < 127)
                return {0, 2};
            return {-1, 5};
        case 1:
            return {-1, 5};
        case 2:
            if (a == 39)
                return {1, 1};
            return {-1, 5};
        case 4:
            if (a == 39)
                return {0, 2};
            if (a == 92)
                return {0, 2};
            if (a == 110)
                return {0, 2};
            if (a == 116)
                return {0, 2};
            return {-1, 5};
        default:
            return {-1, 5};
        }
    }
};

struct String: public BaseRecognizer {
    String() : BaseRecognizer(Token::STRING, false, 4) {}

    std::pair<int, int> next(int q, uint32_t a) const
    {
        switch (q) {
        case 0:
            if (a == 34)
                return {0, 2};
            return {-1, 4};
        case 2:
            if (32 <= a && a < 34)
                return {0, 2};
            if (a == 34)
                return {1, 1};
            if (35 <= a && a < 92)
                return {0, 2};
            if (a == 92)
                return {0, 3};
            if (93 <= a && a < 127)
                return {0, 2};
            return {-1, 4};
        case 1:
            return {-1, 4};
        case 3:
            if (32 <= a && a < 34)
                return {0, 2};
            if (a == 34)
                return {0, 2};
            if (35 <= a && a < 92)
                return {0, 2};
            if (a == 92)
                return {0, 2};
            if (93 <= a && a < 127)
                return {0, 2};
            return {-1, 4};
        default:
            return {-1, 4};
        }
    }
};

struct Dot: public BaseRecognizer {
    Dot() : BaseRecognizer(Token::DOT, false, 2) {}

    std::pair<int, int> next(int q, uint32_t a) const
    {
        switch (q) {
        case 0:
            if (a == 46)
                return {1, 1};
            return {-1, 2};
        case 1:
            return {-1, 2};
        default:
            return {-1, 2};
        }
    }
};

struct Lparen: public BaseRecognizer {
    Lparen() : BaseRecognizer(Token::LPAREN, false, 2) {}

    std::pair<int, int> next(int q, uint32_t a) const
    {
        switch (q) {
        case 0:
            if (a == 40)
                return {1, 1};
            return {-1, 2};
        case 1:
            return {-1, 2};
        default:
            return {-1, 2};
        }
    }
};

struct Rparen: public BaseRecognizer {
    Rparen() : BaseRecognizer(Token::RPAREN, false, 2) {}

    std::pair<int, int> next(int q, uint32_t a) const
    {
        switch (q) {
        case 0:
            if (a == 41)
                return {1, 1};
            return {-1, 2};
        case 1:
            return {-1, 2};
        default:
            return {-1, 2};
        }
    }
};

struct Comma: public BaseRecognizer {
    Comma() : BaseRecognizer(Token::COMMA, false, 2) {}

    std::pair<int, int> next(int q, uint32_t a) const
    {
        switch (q) {
        case 0:
            if (a == 44)
                return {1, 1};
            return {-1, 2};
        case 1:
            return {-1, 2};
        default:
            return {-1, 2};
        }
    }
};

struct Star: public BaseRecognizer {
    Star() : BaseRecognizer(Token::STAR, false, 2) {}

    std::pair<int, int> next(int q, uint32_t a) const
    {
        switch (q) {
        case 0:
            if (a == 42)
                return {1, 1};
            return {-1, 2};
        case 1:
            return {-1, 2};
        default:
            return {-1, 2};
        }
    }
};

struct Equal: public BaseRecognizer {
    Equal() : BaseRecognizer(Token::EQUAL, false, 2) {}

    std::pair<int, int> next(int q, uint32_t a) const
    {
        switch (q) {
        case 0:
            if (a == 61)
                return {1, 1};
            return {-1, 2};
        case 1:
            return {-1, 2};
        default:
            return {-1, 2};
        }
    }
};

struct Lbrace: public BaseRecognizer {
    Lbrace() : BaseRecognizer(Token::LBRACE, false, 2) {}

    std::pair<int, int> next(int q, uint32_t a) const
    {
        switch (q) {
        case 0:
            if (a == 123)
                return {1, 1};
            return {-1, 2};
        case 1:
            return {-1, 2};
        default:
            return {-1, 2};
        }
    }
};

struct Rbrace: public BaseRecognizer {
    Rbrace() : BaseRecognizer(Token::RBRACE, false, 2) {}

    std::pair<int, int> next(int q, uint32_t a) const
    {
        switch (q) {
        case 0:
            if (a == 125)
                return {1, 1};
            return {-1, 2};
        case 1:
            return {-1, 2};
        default:
            return {-1, 2};
        }
    }
};

struct Colon: public BaseRecognizer {
    Colon() : BaseRecognizer(Token::COLON, false, 2) {}

    std::pair<int, int> next(int q, uint32_t a) const
    {
        switch (q) {
        case 0:
            if (a == 58)
                return {1, 1};
            return {-1, 2};
        case 1:
            return {-1, 2};
        default:
            return {-1, 2};
        }
    }
};

struct Lbracket: public BaseRecognizer {
    Lbracket() : BaseRecognizer(Token::LBRACKET, false, 2) {}

    std::pair<int, int> next(int q, uint32_t a) const
    {
        switch (q) {
        case 0:
            if (a == 91)
                return {1, 1};
            return {-1, 2};
        case 1:
            return {-1, 2};
        default:
            return {-1, 2};
        }
    }
};

struct Rbracket: public BaseRecognizer {
    Rbracket() : BaseRecognizer(Token::RBRACKET, false, 2) {}

    std::pair<int, int> next(int q, uint32_t a) const
    {
        switch (q) {
        case 0:
            if (a == 93)
                return {1, 1};
            return {-1, 2};
        case 1:
            return {-1, 2};
        default:
            return {-1, 2};
        }
    }
};

struct Plus: public BaseRecognizer {
    Plus() : BaseRecognizer(Token::PLUS, false, 2) {}

    std::pair<int, int> next(int q, uint32_t a) const
    {
        switch (q) {
        case 0:
            if (a == 43)
                return {1, 1};
            return {-1, 2};
        case 1:
            return {-1, 2};
        default:
            return {-1, 2};
        }
    }
};

struct Minus: public BaseRecognizer {
    Minus() : BaseRecognizer(Token::MINUS, false, 2) {}

    std::pair<int, int> next(int q, uint32_t a) const
    {
        switch (q) {
        case 0:
            if (a == 45)
                return {1, 1};
            return {-1, 2};
        case 1:
            return {-1, 2};
        default:
            return {-1, 2};
        }
    }
};

struct Slash: public BaseRecognizer {
    Slash() : BaseRecognizer(Token::SLASH, false, 2) {}

    std::pair<int, int> next(int q, uint32_t a) const
    {
        switch (q) {
        case 0:
            if (a == 47)
                return {1, 1};
            return {-1, 2};
        case 1:
            return {-1, 2};
        default:
            return {-1, 2};
        }
    }
};

struct Lessthan: public BaseRecognizer {
    Lessthan() : BaseRecognizer(Token::LESSTHAN, false, 2) {}

    std::pair<int, int> next(int q, uint32_t a) const
    {
        switch (q) {
        case 0:
            if (a == 60)
                return {1, 1};
            return {-1, 2};
        case 1:
            return {-1, 2};
        default:
            return {-1, 2};
        }
    }
};

struct Greaterthan: public BaseRecognizer {
    Greaterthan() : BaseRecognizer(Token::GREATERTHAN, false, 2) {}

    std::pair<int, int> next(int q, uint32_t a) const
    {
        switch (q) {
        case 0:
            if (a == 62)
                return {1, 1};
            return {-1, 2};
        case 1:
            return {-1, 2};
        default:
            return {-1, 2};
        }
    }
};

template <class... Recognizers>
std::pair<Token, std::string> match_longest(InputStack& in)
{
    std::unique_ptr<BaseRecognizer> recs[] = {
        std::make_unique<Recognizers>() ...
    };
    Token token = Token::EMPTY;
    std::string lexeme;
    for (auto& r: recs) {
        auto [ok, s] = r->match(in);
        if (ok && s.size() > lexeme.size()) {
            token = r->token;
            lexeme = s;
        }
        for (auto it = s.rbegin(); it != s.rend(); ++it) {
            in.unget(*it);
        }
    }
    for (auto it = lexeme.begin(); it != lexeme.end(); ++it) {
        in.get();
    }
    return {token, lexeme};
}

template <class... Recognizers>
std::pair<Token, std::string> match_longest(std::istream& file = std::cin)
{
    InputStack in(file);
    return match_longest<Recognizers...>(in);
}
