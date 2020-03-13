#pragma once
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#define ALL_RECOGNIZERS \
    A, \
    B, \
    Whitespace


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
    char const * const token;

    BaseRecognizer(
        char const *token = "empty",
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

struct A: public BaseRecognizer {
    A() : BaseRecognizer("a", false, 2) {}

    std::pair<int, int> next(int q, uint32_t a) const
    {
        switch (q) {
        case 0:
            if (a == 97)
                return {1, 1};
            return {-1, 2};
        case 1:
            return {-1, 2};
        default:
            return {-1, 2};
        }
    }
};

struct B: public BaseRecognizer {
    B() : BaseRecognizer("b", false, 2) {}

    std::pair<int, int> next(int q, uint32_t a) const
    {
        switch (q) {
        case 0:
            if (a == 98)
                return {1, 1};
            return {-1, 2};
        case 1:
            return {-1, 2};
        default:
            return {-1, 2};
        }
    }
};

struct Whitespace: public BaseRecognizer {
    Whitespace() : BaseRecognizer("whitespace", false, 2) {}

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

template <class... Recognizers>
std::pair<char const*, std::string> match_longest(InputStack& in)
{
    std::unique_ptr<BaseRecognizer> recs[] = {
        std::make_unique<Recognizers>() ...
    };
    char const *token = "empty";
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
std::pair<char const*, std::string> match_longest(std::istream& file = std::cin)
{
    InputStack in(file);
    return match_longest<Recognizers...>(in);
}


