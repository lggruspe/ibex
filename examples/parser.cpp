extern "C" {
    #include "scanner.h"
}
#include "parser.h"
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

const std::set<std::string> ignores = {
    "whitespace",
};

enum class Action { ERROR = 0, GOTO, SHIFT, REDUCE, ACCEPT };

std::map<int, std::map<std::string, std::pair<Action, int>>> parse_table {
    {
        0, {
            {"A", {Action::GOTO, 1}},
            {"a", {Action::SHIFT, 2}},
            {"empty", {Action::REDUCE, 1}},
        }
    },
    {
        1, {
            {"empty", {Action::ACCEPT, 0}},
        }
    },
    {
        2, {
            {"A", {Action::GOTO, 3}},
            {"a", {Action::SHIFT, 4}},
            {"b", {Action::REDUCE, 1}},
        }
    },
    {
        3, {
            {"b", {Action::SHIFT, 5}},
        }
    },
    {
        4, {
            {"A", {Action::GOTO, 6}},
            {"a", {Action::SHIFT, 4}},
            {"b", {Action::REDUCE, 1}},
        }
    },
    {
        5, {
            {"empty", {Action::REDUCE, 0}},
        }
    },
    {
        6, {
            {"b", {Action::SHIFT, 7}},
        }
    },
    {
        7, {
            {"b", {Action::REDUCE, 0}},
        }
    },
};

// NULL terminated
std::vector<std::pair<char const*, std::vector<char const*>>> rules {
    {"A", { "a", "A", "b", nullptr }},
    {"A", { nullptr }},
    {"S", { "A", nullptr }},
};

std::pair<std::string, std::string> scan(const char*& text)
{
    int n;
    std::string tok = match_longest(text, &n);
    while (ignores.count(tok)) {
        text += n;
        tok = match_longest(text, &n);
    }
    if (tok.empty()) {
        tok = *text ? "error" : "empty";
    }
    std::string lex(text, n);
    text += n;
    return {tok, lex};
}

bool parse(
    char const *text,
    bool (*shift)(void*, char const*, char const*),
    bool (*reduce)(void*, char const*, char const* const*),
    void *arg)
{
    std::vector<int> states {0};
    auto lookahead = scan(text);
    for (;;) {
        auto tok = lookahead.first;
        auto lex = lookahead.second;
        auto action = parse_table[states.back()][tok];
        switch (action.first) {
        case Action::ACCEPT:
            return true;
        case Action::SHIFT:
            if (shift && !shift(arg, tok.c_str(), lex.c_str())) {
                return false;
            }
            states.push_back(action.second);
            lookahead = scan(text);
            break;
        case Action::REDUCE: {
            const auto& rule = rules[action.second];
            if (reduce && !reduce(arg, rule.first, &rule.second[0])) {
                return false;
            }
            for (const auto& s: rule.second) {
                if (!s) {
                    break;
                }
                states.pop_back();
            }
            states.push_back(parse_table[states.back()][rule.first].second);
            break;
        }
        default:
            return false;
        }
    }
}
