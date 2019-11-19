#pragma once
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#define ALL_RECOGNIZERS \
    {%- for scanner in scanners %}
    {{ scanner.token|title }}{% if loop.index != loop.length %}, \{% endif -%}
    {% endfor %}

enum class Token {
    EMPTY = 0,
    {%- for scanner in scanners %}
    {{ scanner.token|upper }},
    {%- endfor %}
};

std::ostream& operator<<(std::ostream& out, Token token)
{
    switch (token) {
    case Token::EMPTY:
        return out << "empty";
    {%- for scanner in scanners %}
    case Token::{{ scanner.token|upper }}:
        return out << "{{ scanner.token }}";
    {%- endfor %}
    default:
        return out;
    }
}

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

    //virtual ~BaseRecognizer() {}    // TODO is this needed?
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
{% for scanner in scanners %}
{% include "scanner.cpp" %}
{% endfor %}
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
    return {Token::EMPTY, ""};
}

template <class... Recognizers>
std::pair<Token, std::string> match_longest(std::istream& in = std::cin)
{
    std::vector<std::unique_ptr<BaseRecognizer>> recs = {
        std::make_unique<Recognizers>() ...
    };
    Token token = Token::EMPTY;
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
