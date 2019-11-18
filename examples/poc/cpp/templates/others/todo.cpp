#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

enum class Token {
    INVALID = 0,
    IDENTIFIER,
    INTEGER,
};

struct BaseRecognizer {
    Token token;

    BaseRecognizer(
        Token token = Token::INVALID,
        bool accept = false, // should be true if 0 is an accept state
        int error = -1)
        : token(token)
        , accept(accept)
        , error(error)
    {}

    virtual std::pair<int, int> next(int q, uint32_t a) const = 0;

    std::pair<bool, std::string> match(std::istream& in = std::cin)
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
            in.unget();
            lexeme.pop_back();
        }
        return { accept, std::string(lexeme.begin(), lexeme.end()) };
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
            if ('a' <= a && a < 'z'+1) {
                return {1, 1};
            }
            if ('A' <= a && a < 'Z'+1) {
                return {1, 1};
            }
            if ('_' == a) {
                return {1, 1};
            }
            return {-1, 2};
        case 1:
            if ('a' <= a && a < 'z'+1) {
                return {1, 1};
            }
            if ('A' <= a && a < 'Z'+1) {
                return {1, 1};
            }
            if ('_' == a) {
                return {1, 1};
            }
            if ('0' <= a && a <'9'+1) {
                return {1, 1};
            }
            return {-1, 2};
        default:
            return {-1, 2};
        }
    }
};

template <class... Recognizers>
std::pair<Token, std::string> match_first(std::istream& in = std::cin)
{
    std::vector<std::unique_ptr<BaseRecognizer>> recs = {
        std::make_unique<Recognizers>() ...
    };
    for (auto r: recs) {
        auto [ok, s] = r->match(in);
        if (ok) {
            return {r->token, s};
        }
    }
    return {Token::INVALID, ""};
}

template <class... Recognizers>
std::pair<Token, std::string> match_longest(std::istream& in = std::cin)
{
    std::vector<std::unique_ptr<BaseRecognizer>> recs = {
        std::make_unique<Recognizers>() ...
    };
    Token token = Token::INVALID;
    std::string lexeme;
    for (auto r: recs) {
        auto [ok, s] = r->match(in);
        if (ok && s.size() > lexeme.size()) {
            token = r->token;
            lexeme = s;
        }
    }
    return {token, lexeme};
}

int main()
{
}
