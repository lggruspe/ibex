#pragma once
#include "scanner.hpp"
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
namespace parser
{
using namespace scanner;

enum class Action { ERROR = 0, GOTO, SHIFT, REDUCE, ACCEPT };

std::map<int, std::map<std::string, std::pair<Action, int>>> table {
    {
        0, {
            {"compound", {Action::GOTO, 1}},
            {"dot", {Action::SHIFT, 2}},
            {"expr", {Action::GOTO, 3}},
            {"factor", {Action::GOTO, 4}},
            {"lbracket", {Action::SHIFT, 5}},
            {"lparen", {Action::SHIFT, 6}},
            {"simple", {Action::GOTO, 7}},
            {"symbol", {Action::SHIFT, 8}},
            {"term", {Action::GOTO, 9}},
            {"value", {Action::GOTO, 10}},
        }
    },
    {
        1, {
            {"dot", {Action::REDUCE, 18}},
            {"empty", {Action::REDUCE, 18}},
            {"lbracket", {Action::REDUCE, 18}},
            {"lparen", {Action::REDUCE, 18}},
            {"pipe", {Action::REDUCE, 18}},
            {"plus", {Action::REDUCE, 18}},
            {"question", {Action::REDUCE, 18}},
            {"star", {Action::REDUCE, 18}},
            {"symbol", {Action::REDUCE, 18}},
        }
    },
    {
        2, {
            {"dot", {Action::REDUCE, 12}},
            {"empty", {Action::REDUCE, 12}},
            {"lbracket", {Action::REDUCE, 12}},
            {"lparen", {Action::REDUCE, 12}},
            {"pipe", {Action::REDUCE, 12}},
            {"plus", {Action::REDUCE, 12}},
            {"question", {Action::REDUCE, 12}},
            {"star", {Action::REDUCE, 12}},
            {"symbol", {Action::REDUCE, 12}},
        }
    },
    {
        3, {
            {"empty", {Action::ACCEPT, 0}},
            {"pipe", {Action::SHIFT, 36}},
        }
    },
    {
        4, {
            {"dot", {Action::REDUCE, 16}},
            {"empty", {Action::REDUCE, 16}},
            {"lbracket", {Action::REDUCE, 16}},
            {"lparen", {Action::REDUCE, 16}},
            {"pipe", {Action::REDUCE, 16}},
            {"symbol", {Action::REDUCE, 16}},
        }
    },
    {
        5, {
            {"element", {Action::GOTO, 24}},
            {"list", {Action::GOTO, 25}},
            {"symbol", {Action::SHIFT, 26}},
        }
    },
    {
        6, {
            {"compound", {Action::GOTO, 12}},
            {"dot", {Action::SHIFT, 13}},
            {"expr", {Action::GOTO, 14}},
            {"factor", {Action::GOTO, 15}},
            {"lbracket", {Action::SHIFT, 16}},
            {"lparen", {Action::SHIFT, 17}},
            {"simple", {Action::GOTO, 18}},
            {"symbol", {Action::SHIFT, 19}},
            {"term", {Action::GOTO, 20}},
            {"value", {Action::GOTO, 21}},
        }
    },
    {
        7, {
            {"dot", {Action::REDUCE, 17}},
            {"empty", {Action::REDUCE, 17}},
            {"lbracket", {Action::REDUCE, 17}},
            {"lparen", {Action::REDUCE, 17}},
            {"pipe", {Action::REDUCE, 17}},
            {"plus", {Action::REDUCE, 17}},
            {"question", {Action::REDUCE, 17}},
            {"star", {Action::REDUCE, 17}},
            {"symbol", {Action::REDUCE, 17}},
        }
    },
    {
        8, {
            {"dot", {Action::REDUCE, 13}},
            {"empty", {Action::REDUCE, 13}},
            {"lbracket", {Action::REDUCE, 13}},
            {"lparen", {Action::REDUCE, 13}},
            {"pipe", {Action::REDUCE, 13}},
            {"plus", {Action::REDUCE, 13}},
            {"question", {Action::REDUCE, 13}},
            {"star", {Action::REDUCE, 13}},
            {"symbol", {Action::REDUCE, 13}},
        }
    },
    {
        9, {
            {"compound", {Action::GOTO, 1}},
            {"dot", {Action::SHIFT, 2}},
            {"empty", {Action::REDUCE, 5}},
            {"factor", {Action::GOTO, 11}},
            {"lbracket", {Action::SHIFT, 5}},
            {"lparen", {Action::SHIFT, 6}},
            {"pipe", {Action::REDUCE, 5}},
            {"simple", {Action::GOTO, 7}},
            {"symbol", {Action::SHIFT, 8}},
            {"value", {Action::GOTO, 10}},
        }
    },
    {
        10, {
            {"dot", {Action::REDUCE, 9}},
            {"empty", {Action::REDUCE, 9}},
            {"lbracket", {Action::REDUCE, 9}},
            {"lparen", {Action::REDUCE, 9}},
            {"pipe", {Action::REDUCE, 9}},
            {"plus", {Action::SHIFT, 37}},
            {"question", {Action::SHIFT, 38}},
            {"star", {Action::SHIFT, 39}},
            {"symbol", {Action::REDUCE, 9}},
        }
    },
    {
        11, {
            {"dot", {Action::REDUCE, 15}},
            {"empty", {Action::REDUCE, 15}},
            {"lbracket", {Action::REDUCE, 15}},
            {"lparen", {Action::REDUCE, 15}},
            {"pipe", {Action::REDUCE, 15}},
            {"symbol", {Action::REDUCE, 15}},
        }
    },
    {
        12, {
            {"dot", {Action::REDUCE, 18}},
            {"lbracket", {Action::REDUCE, 18}},
            {"lparen", {Action::REDUCE, 18}},
            {"pipe", {Action::REDUCE, 18}},
            {"plus", {Action::REDUCE, 18}},
            {"question", {Action::REDUCE, 18}},
            {"rparen", {Action::REDUCE, 18}},
            {"star", {Action::REDUCE, 18}},
            {"symbol", {Action::REDUCE, 18}},
        }
    },
    {
        13, {
            {"dot", {Action::REDUCE, 12}},
            {"lbracket", {Action::REDUCE, 12}},
            {"lparen", {Action::REDUCE, 12}},
            {"pipe", {Action::REDUCE, 12}},
            {"plus", {Action::REDUCE, 12}},
            {"question", {Action::REDUCE, 12}},
            {"rparen", {Action::REDUCE, 12}},
            {"star", {Action::REDUCE, 12}},
            {"symbol", {Action::REDUCE, 12}},
        }
    },
    {
        14, {
            {"pipe", {Action::SHIFT, 31}},
            {"rparen", {Action::SHIFT, 32}},
        }
    },
    {
        15, {
            {"dot", {Action::REDUCE, 16}},
            {"lbracket", {Action::REDUCE, 16}},
            {"lparen", {Action::REDUCE, 16}},
            {"pipe", {Action::REDUCE, 16}},
            {"rparen", {Action::REDUCE, 16}},
            {"symbol", {Action::REDUCE, 16}},
        }
    },
    {
        16, {
            {"element", {Action::GOTO, 24}},
            {"list", {Action::GOTO, 27}},
            {"symbol", {Action::SHIFT, 26}},
        }
    },
    {
        17, {
            {"compound", {Action::GOTO, 12}},
            {"dot", {Action::SHIFT, 13}},
            {"expr", {Action::GOTO, 22}},
            {"factor", {Action::GOTO, 15}},
            {"lbracket", {Action::SHIFT, 16}},
            {"lparen", {Action::SHIFT, 17}},
            {"simple", {Action::GOTO, 18}},
            {"symbol", {Action::SHIFT, 19}},
            {"term", {Action::GOTO, 20}},
            {"value", {Action::GOTO, 21}},
        }
    },
    {
        18, {
            {"dot", {Action::REDUCE, 17}},
            {"lbracket", {Action::REDUCE, 17}},
            {"lparen", {Action::REDUCE, 17}},
            {"pipe", {Action::REDUCE, 17}},
            {"plus", {Action::REDUCE, 17}},
            {"question", {Action::REDUCE, 17}},
            {"rparen", {Action::REDUCE, 17}},
            {"star", {Action::REDUCE, 17}},
            {"symbol", {Action::REDUCE, 17}},
        }
    },
    {
        19, {
            {"dot", {Action::REDUCE, 13}},
            {"lbracket", {Action::REDUCE, 13}},
            {"lparen", {Action::REDUCE, 13}},
            {"pipe", {Action::REDUCE, 13}},
            {"plus", {Action::REDUCE, 13}},
            {"question", {Action::REDUCE, 13}},
            {"rparen", {Action::REDUCE, 13}},
            {"star", {Action::REDUCE, 13}},
            {"symbol", {Action::REDUCE, 13}},
        }
    },
    {
        20, {
            {"compound", {Action::GOTO, 12}},
            {"dot", {Action::SHIFT, 13}},
            {"factor", {Action::GOTO, 23}},
            {"lbracket", {Action::SHIFT, 16}},
            {"lparen", {Action::SHIFT, 17}},
            {"pipe", {Action::REDUCE, 5}},
            {"rparen", {Action::REDUCE, 5}},
            {"simple", {Action::GOTO, 18}},
            {"symbol", {Action::SHIFT, 19}},
            {"value", {Action::GOTO, 21}},
        }
    },
    {
        21, {
            {"dot", {Action::REDUCE, 9}},
            {"lbracket", {Action::REDUCE, 9}},
            {"lparen", {Action::REDUCE, 9}},
            {"pipe", {Action::REDUCE, 9}},
            {"plus", {Action::SHIFT, 40}},
            {"question", {Action::SHIFT, 41}},
            {"rparen", {Action::REDUCE, 9}},
            {"star", {Action::SHIFT, 42}},
            {"symbol", {Action::REDUCE, 9}},
        }
    },
    {
        22, {
            {"pipe", {Action::SHIFT, 31}},
            {"rparen", {Action::SHIFT, 33}},
        }
    },
    {
        23, {
            {"dot", {Action::REDUCE, 15}},
            {"lbracket", {Action::REDUCE, 15}},
            {"lparen", {Action::REDUCE, 15}},
            {"pipe", {Action::REDUCE, 15}},
            {"rparen", {Action::REDUCE, 15}},
            {"symbol", {Action::REDUCE, 15}},
        }
    },
    {
        24, {
            {"rbracket", {Action::REDUCE, 11}},
            {"symbol", {Action::REDUCE, 11}},
        }
    },
    {
        25, {
            {"element", {Action::GOTO, 28}},
            {"rbracket", {Action::SHIFT, 29}},
            {"symbol", {Action::SHIFT, 26}},
        }
    },
    {
        26, {
            {"dash", {Action::SHIFT, 34}},
            {"rbracket", {Action::REDUCE, 2}},
            {"symbol", {Action::REDUCE, 2}},
        }
    },
    {
        27, {
            {"element", {Action::GOTO, 28}},
            {"rbracket", {Action::SHIFT, 30}},
            {"symbol", {Action::SHIFT, 26}},
        }
    },
    {
        28, {
            {"rbracket", {Action::REDUCE, 10}},
            {"symbol", {Action::REDUCE, 10}},
        }
    },
    {
        29, {
            {"dot", {Action::REDUCE, 1}},
            {"empty", {Action::REDUCE, 1}},
            {"lbracket", {Action::REDUCE, 1}},
            {"lparen", {Action::REDUCE, 1}},
            {"pipe", {Action::REDUCE, 1}},
            {"plus", {Action::REDUCE, 1}},
            {"question", {Action::REDUCE, 1}},
            {"star", {Action::REDUCE, 1}},
            {"symbol", {Action::REDUCE, 1}},
        }
    },
    {
        30, {
            {"dot", {Action::REDUCE, 1}},
            {"lbracket", {Action::REDUCE, 1}},
            {"lparen", {Action::REDUCE, 1}},
            {"pipe", {Action::REDUCE, 1}},
            {"plus", {Action::REDUCE, 1}},
            {"question", {Action::REDUCE, 1}},
            {"rparen", {Action::REDUCE, 1}},
            {"star", {Action::REDUCE, 1}},
            {"symbol", {Action::REDUCE, 1}},
        }
    },
    {
        31, {
            {"compound", {Action::GOTO, 12}},
            {"dot", {Action::SHIFT, 13}},
            {"factor", {Action::GOTO, 15}},
            {"lbracket", {Action::SHIFT, 16}},
            {"lparen", {Action::SHIFT, 17}},
            {"simple", {Action::GOTO, 18}},
            {"symbol", {Action::SHIFT, 19}},
            {"term", {Action::GOTO, 44}},
            {"value", {Action::GOTO, 21}},
        }
    },
    {
        32, {
            {"dot", {Action::REDUCE, 0}},
            {"empty", {Action::REDUCE, 0}},
            {"lbracket", {Action::REDUCE, 0}},
            {"lparen", {Action::REDUCE, 0}},
            {"pipe", {Action::REDUCE, 0}},
            {"plus", {Action::REDUCE, 0}},
            {"question", {Action::REDUCE, 0}},
            {"star", {Action::REDUCE, 0}},
            {"symbol", {Action::REDUCE, 0}},
        }
    },
    {
        33, {
            {"dot", {Action::REDUCE, 0}},
            {"lbracket", {Action::REDUCE, 0}},
            {"lparen", {Action::REDUCE, 0}},
            {"pipe", {Action::REDUCE, 0}},
            {"plus", {Action::REDUCE, 0}},
            {"question", {Action::REDUCE, 0}},
            {"rparen", {Action::REDUCE, 0}},
            {"star", {Action::REDUCE, 0}},
            {"symbol", {Action::REDUCE, 0}},
        }
    },
    {
        34, {
            {"symbol", {Action::SHIFT, 35}},
        }
    },
    {
        35, {
            {"rbracket", {Action::REDUCE, 3}},
            {"symbol", {Action::REDUCE, 3}},
        }
    },
    {
        36, {
            {"compound", {Action::GOTO, 1}},
            {"dot", {Action::SHIFT, 2}},
            {"factor", {Action::GOTO, 4}},
            {"lbracket", {Action::SHIFT, 5}},
            {"lparen", {Action::SHIFT, 6}},
            {"simple", {Action::GOTO, 7}},
            {"symbol", {Action::SHIFT, 8}},
            {"term", {Action::GOTO, 43}},
            {"value", {Action::GOTO, 10}},
        }
    },
    {
        37, {
            {"dot", {Action::REDUCE, 7}},
            {"empty", {Action::REDUCE, 7}},
            {"lbracket", {Action::REDUCE, 7}},
            {"lparen", {Action::REDUCE, 7}},
            {"pipe", {Action::REDUCE, 7}},
            {"symbol", {Action::REDUCE, 7}},
        }
    },
    {
        38, {
            {"dot", {Action::REDUCE, 8}},
            {"empty", {Action::REDUCE, 8}},
            {"lbracket", {Action::REDUCE, 8}},
            {"lparen", {Action::REDUCE, 8}},
            {"pipe", {Action::REDUCE, 8}},
            {"symbol", {Action::REDUCE, 8}},
        }
    },
    {
        39, {
            {"dot", {Action::REDUCE, 6}},
            {"empty", {Action::REDUCE, 6}},
            {"lbracket", {Action::REDUCE, 6}},
            {"lparen", {Action::REDUCE, 6}},
            {"pipe", {Action::REDUCE, 6}},
            {"symbol", {Action::REDUCE, 6}},
        }
    },
    {
        40, {
            {"dot", {Action::REDUCE, 7}},
            {"lbracket", {Action::REDUCE, 7}},
            {"lparen", {Action::REDUCE, 7}},
            {"pipe", {Action::REDUCE, 7}},
            {"rparen", {Action::REDUCE, 7}},
            {"symbol", {Action::REDUCE, 7}},
        }
    },
    {
        41, {
            {"dot", {Action::REDUCE, 8}},
            {"lbracket", {Action::REDUCE, 8}},
            {"lparen", {Action::REDUCE, 8}},
            {"pipe", {Action::REDUCE, 8}},
            {"rparen", {Action::REDUCE, 8}},
            {"symbol", {Action::REDUCE, 8}},
        }
    },
    {
        42, {
            {"dot", {Action::REDUCE, 6}},
            {"lbracket", {Action::REDUCE, 6}},
            {"lparen", {Action::REDUCE, 6}},
            {"pipe", {Action::REDUCE, 6}},
            {"rparen", {Action::REDUCE, 6}},
            {"symbol", {Action::REDUCE, 6}},
        }
    },
    {
        43, {
            {"compound", {Action::GOTO, 1}},
            {"dot", {Action::SHIFT, 2}},
            {"empty", {Action::REDUCE, 4}},
            {"factor", {Action::GOTO, 11}},
            {"lbracket", {Action::SHIFT, 5}},
            {"lparen", {Action::SHIFT, 6}},
            {"pipe", {Action::REDUCE, 4}},
            {"simple", {Action::GOTO, 7}},
            {"symbol", {Action::SHIFT, 8}},
            {"value", {Action::GOTO, 10}},
        }
    },
    {
        44, {
            {"compound", {Action::GOTO, 12}},
            {"dot", {Action::SHIFT, 13}},
            {"factor", {Action::GOTO, 23}},
            {"lbracket", {Action::SHIFT, 16}},
            {"lparen", {Action::SHIFT, 17}},
            {"pipe", {Action::REDUCE, 4}},
            {"rparen", {Action::REDUCE, 4}},
            {"simple", {Action::GOTO, 18}},
            {"symbol", {Action::SHIFT, 19}},
            {"value", {Action::GOTO, 21}},
        }
    },
};

const std::vector<std::pair<std::string, std::vector<std::string>>> rules = {
    {"compound", {"lparen", "expr", "rparen"}},
    {"compound", {"lbracket", "list", "rbracket"}},
    {"element", {"symbol"}},
    {"element", {"symbol", "dash", "symbol"}},
    {"expr", {"expr", "pipe", "term"}},
    {"expr", {"term"}},
    {"factor", {"value", "star"}},
    {"factor", {"value", "plus"}},
    {"factor", {"value", "question"}},
    {"factor", {"value"}},
    {"list", {"list", "element"}},
    {"list", {"element"}},
    {"simple", {"dot"}},
    {"simple", {"symbol"}},
    {"start", {"expr"}},
    {"term", {"term", "factor"}},
    {"term", {"factor"}},
    {"value", {"simple"}},
    {"value", {"compound"}},
};

std::pair<std::string, std::string> scan(InputStack& in)
{
    auto [token, lexeme] = match_longest<ALL_RECOGNIZERS>(in);
    if (std::string(token) == std::string()) {
        uint32_t a = in.get();
        uint32_t eof = std::char_traits<char>::eof();
        if (a != eof) {
            in.unget(a);
            return std::make_pair("error", lexeme);
        }
    }
    return std::make_pair(token, lexeme);
}

struct SyntaxError {
    char const* what() const { return "syntax error"; }
};

template <class T>
struct BaseCallback {
    using ShiftHandler = bool (*)(T&, const std::pair<std::string, std::string>&);
    using ReduceHandler = bool (*)(T&, const std::pair<std::string, std::vector<std::string>>&);

    std::map<std::string, std::vector<ShiftHandler>> shift_handlers;
    std::map<std::string, std::vector<ReduceHandler>> reduce_handlers;
    T state;

    BaseCallback() {}

    BaseCallback(const T& state) : state(state) {}

    void shift(const std::string& token, ShiftHandler handler)
    {
        shift_handlers[token].push_back(handler);
    }

    void reduce(const std::string& rule, ReduceHandler handler)
    {
        reduce_handlers[rule].push_back(handler);
    }

    virtual bool handle_shift(const std::pair<std::string, std::string>& lookahead)
    {
        auto it = shift_handlers.find(lookahead.first);
        if (it == shift_handlers.end()) {
            return false;
        }
        for (auto f: it->second) {
            if (bool ok = f(state, lookahead); !ok) {
                return false;
            }
        }
        return true;
    }

    virtual bool handle_reduce(const std::pair<std::string, std::vector<std::string>>& rule)
    {
        std::string key = rule.first + " ->";
        for (const auto& word: rule.second) {
            key += " " + word;
        }
        auto it = reduce_handlers.find(key);
        if (it == reduce_handlers.end()) {
            return false;
        }
        for (auto f: it->second) {
            if (bool ok = f(state, rule); !ok) {
                return false;
            }
        }
        return true;
    }
};

template <class T>
bool parse(
        std::istream& is = std::cin,
        BaseCallback<T>* cb = nullptr)
{
    InputStack input_stack(is);
    std::vector<int> states{0};
    std::pair<std::string, std::string> lookahead = scan(input_stack);
    std::unique_ptr<BaseCallback<T>> cb_ptr;
    if (!cb) {
        cb_ptr = std::make_unique<BaseCallback<T>>();
        cb = cb_ptr.get();
    }
    for (;;) {
        auto action = table[states.back()][lookahead.first];
        switch (action.first) {
        case Action::ACCEPT:
            return true;
        case Action::SHIFT:
            if (bool ok = cb->handle_shift(lookahead); !ok) {
                return false;
            }
            lookahead = scan(input_stack);
            states.push_back(action.second);
            break;
        case Action::REDUCE: {
            const auto& rule = rules[action.second];
            if (bool ok = cb->handle_reduce(rule); !ok) {
                return false;
            }
            for (int i = 0; i < (int)rule.second.size(); ++i) {
                states.pop_back();
            }
            states.push_back(table[states.back()][rule.first].second);
            break;
        }
        default:
            return false;
        }
    }
}

template <class T>
bool parse(
        const std::string& s,
        BaseCallback<T>* cb = nullptr)
{
    std::stringstream ss;
    ss << s;
    return parse(ss, cb);
}
}
