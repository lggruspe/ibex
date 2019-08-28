#pragma once
#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace match {

template <class Scanner>
std::string fsingle(std::istream& in, std::string lexeme="")
{
    Scanner scanner;
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

template <class Scanner>
std::string single(std::string lexeme="")
{
    return fsingle<Scanner>(std::cin, lexeme);
}

template <class Token, class BaseScanner, class... Scanners>
std::pair<Token, std::string> flongest(std::istream& in)
{
    if constexpr(sizeof...(Scanners) == 0) {
        return {Token(), ""};
    }

    std::vector<std::shared_ptr<BaseScanner>> scanners = { std::make_shared<Scanners>() ... };

    Token record_token;
    std::string record_lexeme;
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

    if (!record_lexeme.empty()) {
        std::for_each(record_lexeme.begin(), record_lexeme.end(),
                [&in](const auto&) {
                    in.get();
                });
    }
    return {record_token, record_lexeme};
}

template <class Token, class BaseScanner, class... Scanners>
std::pair<Token, std::string> longest()
{
    return flongest<Token, BaseScanner, Scanners...>(std::cin);
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
            auto [token, lexeme] = flongest<Token, BaseScanner, Scanners...>(*in);
            if (lexeme.empty()) {
                done = true;
            }
            return {token, lexeme};
        }
        return {Token(), ""};
    }
};

}
