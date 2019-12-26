#pragma once
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#define ALL_RECOGNIZERS \
    Pipe, \
    Star, \
    Lparen, \
    Rparen, \
    Dot, \
    Question, \
    Plus, \
    Interval, \
    Symbol

enum class Token {
    EMPTY = 0,
    PIPE,
    STAR,
    LPAREN,
    RPAREN,
    DOT,
    QUESTION,
    PLUS,
    INTERVAL,
    SYMBOL,
};

std::ostream& operator<<(std::ostream& out, Token token)
{
    switch (token) {
    case Token::EMPTY:
        return out << "empty";
    case Token::PIPE:
        return out << "pipe";
    case Token::STAR:
        return out << "star";
    case Token::LPAREN:
        return out << "lparen";
    case Token::RPAREN:
        return out << "rparen";
    case Token::DOT:
        return out << "dot";
    case Token::QUESTION:
        return out << "question";
    case Token::PLUS:
        return out << "plus";
    case Token::INTERVAL:
        return out << "interval";
    case Token::SYMBOL:
        return out << "symbol";
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

struct Pipe: public BaseRecognizer {
    Pipe() : BaseRecognizer(Token::PIPE, false, 2) {}

    std::pair<int, int> next(int q, uint32_t a) const
    {
        switch (q) {
        case 0:
            if (a == 124)
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

struct Question: public BaseRecognizer {
    Question() : BaseRecognizer(Token::QUESTION, false, 2) {}

    std::pair<int, int> next(int q, uint32_t a) const
    {
        switch (q) {
        case 0:
            if (a == 63)
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

struct Interval: public BaseRecognizer {
    Interval() : BaseRecognizer(Token::INTERVAL, false, 10) {}

    std::pair<int, int> next(int q, uint32_t a) const
    {
        switch (q) {
        case 0:
            if (a == 91)
                return {0, 6};
            return {-1, 10};
        case 6:
            if (a < 40)
                return {0, 5};
            if (a == 40)
                return {0, 5};
            if (a == 41)
                return {0, 5};
            if (a == 42)
                return {0, 5};
            if (a == 43)
                return {0, 5};
            if (a == 44)
                return {0, 5};
            if (a == 45)
                return {0, 5};
            if (a == 46)
                return {0, 5};
            if (a == 47)
                return {0, 5};
            if (48 <= a && a < 58)
                return {0, 5};
            if (58 <= a && a < 63)
                return {0, 5};
            if (a == 63)
                return {0, 5};
            if (64 <= a && a < 91)
                return {0, 5};
            if (a == 92)
                return {0, 7};
            if (94 <= a && a < 120)
                return {0, 5};
            if (a == 120)
                return {0, 5};
            if (121 <= a && a < 124)
                return {0, 5};
            if (a == 124)
                return {0, 5};
            if (125 <= a && a < 4294967295)
                return {0, 5};
            return {-1, 10};
        case 1:
            return {-1, 10};
        case 2:
            if (a == 93)
                return {1, 1};
            return {-1, 10};
        case 3:
            if (a < 40)
                return {0, 2};
            if (a == 40)
                return {0, 2};
            if (a == 41)
                return {0, 2};
            if (a == 42)
                return {0, 2};
            if (a == 43)
                return {0, 2};
            if (a == 44)
                return {0, 2};
            if (a == 45)
                return {0, 2};
            if (a == 46)
                return {0, 2};
            if (a == 47)
                return {0, 2};
            if (48 <= a && a < 58)
                return {0, 2};
            if (58 <= a && a < 63)
                return {0, 2};
            if (a == 63)
                return {0, 2};
            if (64 <= a && a < 91)
                return {0, 2};
            if (a == 92)
                return {0, 4};
            if (94 <= a && a < 120)
                return {0, 2};
            if (a == 120)
                return {0, 2};
            if (121 <= a && a < 124)
                return {0, 2};
            if (a == 124)
                return {0, 2};
            if (125 <= a && a < 4294967295)
                return {0, 2};
            return {-1, 10};
        case 4:
            if (a == 40)
                return {0, 2};
            if (a == 41)
                return {0, 2};
            if (a == 42)
                return {0, 2};
            if (a == 43)
                return {0, 2};
            if (a == 46)
                return {0, 2};
            if (a == 63)
                return {0, 2};
            if (a == 91)
                return {0, 2};
            if (a == 92)
                return {0, 2};
            if (a == 93)
                return {0, 2};
            if (a == 120)
                return {0, 8};
            if (a == 124)
                return {0, 2};
            return {-1, 10};
        case 8:
            if (48 <= a && a < 58)
                return {0, 11};
            return {-1, 10};
        case 5:
            if (a == 45)
                return {0, 3};
            return {-1, 10};
        case 7:
            if (a == 40)
                return {0, 5};
            if (a == 41)
                return {0, 5};
            if (a == 42)
                return {0, 5};
            if (a == 43)
                return {0, 5};
            if (a == 46)
                return {0, 5};
            if (a == 63)
                return {0, 5};
            if (a == 91)
                return {0, 5};
            if (a == 92)
                return {0, 5};
            if (a == 93)
                return {0, 5};
            if (a == 120)
                return {0, 9};
            if (a == 124)
                return {0, 5};
            return {-1, 10};
        case 9:
            if (48 <= a && a < 58)
                return {0, 12};
            return {-1, 10};
        case 11:
            if (48 <= a && a < 58)
                return {0, 11};
            if (a == 93)
                return {1, 1};
            return {-1, 10};
        case 12:
            if (a == 45)
                return {0, 3};
            if (48 <= a && a < 58)
                return {0, 12};
            return {-1, 10};
        default:
            return {-1, 10};
        }
    }
};

struct Symbol: public BaseRecognizer {
    Symbol() : BaseRecognizer(Token::SYMBOL, false, 2) {}

    std::pair<int, int> next(int q, uint32_t a) const
    {
        switch (q) {
        case 0:
            if (a < 40)
                return {1, 1};
            if (a == 40)
                return {1, 1};
            if (a == 41)
                return {1, 1};
            if (a == 42)
                return {1, 1};
            if (a == 43)
                return {1, 1};
            if (44 <= a && a < 46)
                return {1, 1};
            if (a == 46)
                return {1, 1};
            if (a == 47)
                return {1, 1};
            if (48 <= a && a < 58)
                return {1, 1};
            if (58 <= a && a < 63)
                return {1, 1};
            if (a == 63)
                return {1, 1};
            if (64 <= a && a < 91)
                return {1, 1};
            if (a == 92)
                return {0, 3};
            if (94 <= a && a < 120)
                return {1, 1};
            if (a == 120)
                return {1, 1};
            if (121 <= a && a < 124)
                return {1, 1};
            if (a == 124)
                return {1, 1};
            if (125 <= a && a < 4294967295)
                return {1, 1};
            return {-1, 2};
        case 1:
            return {-1, 2};
        case 3:
            if (a == 40)
                return {1, 1};
            if (a == 41)
                return {1, 1};
            if (a == 42)
                return {1, 1};
            if (a == 43)
                return {1, 1};
            if (a == 46)
                return {1, 1};
            if (a == 63)
                return {1, 1};
            if (a == 91)
                return {1, 1};
            if (a == 92)
                return {1, 1};
            if (a == 93)
                return {1, 1};
            if (a == 120)
                return {0, 4};
            if (a == 124)
                return {1, 1};
            return {-1, 2};
        case 4:
            if (48 <= a && a < 58)
                return {1, 5};
            return {-1, 2};
        case 5:
            if (48 <= a && a < 58)
                return {1, 5};
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
