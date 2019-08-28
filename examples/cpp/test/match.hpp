#pragma once
#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace match {

template <class Scanner>
std::string single(std::istream& in=std::cin)
{
    Scanner scanner;
    std::string lexeme;
    for (int c = in.get(); c != EOF; c = in.get()) {
        lexeme += (char)c;
        if (!scanner.next(c)) {
            for (auto it = lexeme.rbegin(); it != lexeme.rend(); ++it) {
                in.putback((int)(*it));
            }
            for (int i = 0; i < scanner.backtrack_steps(); ++i) {
                lexeme.pop_back();
            }
            break;
        }
    }
    return lexeme;
}

template <class Token, class BaseScanner, class... Scanners>
std::pair<Token, std::string> longest(std::istream& in=std::cin)
{
    Token record_token = static_cast<Token>(0);
    std::string record_lexeme;
    if constexpr(sizeof...(Scanners) == 0) {
        return {record_token, record_lexeme};
    }

    std::vector<std::shared_ptr<BaseScanner>> scanners = { std::make_shared<Scanners>() ... };

    for (auto& scanner_ptr: scanners) {
        std::string lexeme;
        for (int c = in.get(); c != EOF; c = in.get()) {
            lexeme += (char)c;
            if (!scanner_ptr->next(c)) {
                for (auto it = lexeme.rbegin(); it != lexeme.rend(); ++it) {
                    in.putback((int)(*it));
                }
                for (int i = 0; i < scanner_ptr->backtrack_steps(); ++i) {
                    lexeme.pop_back();
                }
                if (scanner_ptr->accepts) {
                    if (lexeme.size() > record_lexeme.size()) {
                        record_token = scanner_ptr->token;
                        record_lexeme = lexeme;
                    }
                }
                break;
            }
        }
    }
    return {record_token, record_lexeme};
}

template <class Token, class BaseScanner, class... Scanners>
struct Tokenizer {
    std::istream* in;
    bool done;

    Tokenizer(std::istream& in=std::cin)
        : in(&in)
        , done(false)
    {}

    std::pair<Token, std::string> tokenize()
    {
        if (!done) {
            auto [token, lexeme] = longest<Token, BaseScanner, Scanners...>(*in);
            if (lexeme.empty()) {
                done = true;
            } else {
                std::for_each(record_lexeme.begin(), record_lexeme.end(),
                        [&in](const auto&) {
                            in.get();
                        });
            }
            return {token, lexeme};
        }
        return {static_cast<Token>(0), ""};
    }
};

}
