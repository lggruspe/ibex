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
            {"Rules", {Action::GOTO, 1}},
            {"empty", {Action::REDUCE, 6}},
            {"identifier", {Action::REDUCE, 6}},
        }
    },
    {
        1, {
            {"Lhs", {Action::GOTO, 2}},
            {"Rule", {Action::GOTO, 3}},
            {"empty", {Action::ACCEPT, 0}},
            {"identifier", {Action::SHIFT, 4}},
        }
    },
    {
        2, {
            {"arrow", {Action::SHIFT, 5}},
        }
    },
    {
        3, {
            {"empty", {Action::REDUCE, 5}},
            {"identifier", {Action::REDUCE, 5}},
        }
    },
    {
        4, {
            {"arrow", {Action::REDUCE, 1}},
        }
    },
    {
        5, {
            {"Rhs", {Action::GOTO, 6}},
            {"dot", {Action::REDUCE, 3}},
            {"identifier", {Action::REDUCE, 3}},
        }
    },
    {
        6, {
            {"dot", {Action::SHIFT, 7}},
            {"identifier", {Action::SHIFT, 8}},
        }
    },
    {
        7, {
            {"empty", {Action::REDUCE, 4}},
            {"identifier", {Action::REDUCE, 4}},
        }
    },
    {
        8, {
            {"dot", {Action::REDUCE, 2}},
            {"identifier", {Action::REDUCE, 2}},
        }
    },
};

const std::vector<std::pair<std::string, std::vector<std::string>>> rules = {
    {"Grammar", {"Rules"}},
    {"Lhs", {"identifier"}},
    {"Rhs", {"Rhs", "identifier"}},
    {"Rhs", {}},
    {"Rule", {"Lhs", "arrow", "Rhs", "dot"}},
    {"Rules", {"Rules", "Rule"}},
    {"Rules", {}},
};

std::pair<std::string, std::string> scan(InputStack& in)
{
    auto [token, lexeme] = match_longest<ALL_RECOGNIZERS>(in);
    if (token == "whitespace") {
        return scan(in);
    }
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
