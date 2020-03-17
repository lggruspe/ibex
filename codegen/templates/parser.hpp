#pragma once
{% if config and config.scanner_file %}
#include "{{ config.scanner_file }}"
{% else %}
#include "scanner.hpp"
{% endif %}
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

enum class Action { ERROR = 0, GOTO, SHIFT, REDUCE, ACCEPT };

{% if config and config.parser_namespace %}
namespace {{ config.parser_namespace }}
{
{% endif %}

{% if config and config.scanner_namespace %}
using namespace {{ config.scanner_namespace }};
{% endif %}

std::map<int, std::map<std::string, std::pair<Action, int>>> table {
    {% for row in table %}
    {
        {{ row }}, {
            {% for entry in table[row] %}
            {"{{ entry.symbol }}", {Action::{{ entry.type|upper }}, {{ entry.value }}}},
            {% endfor %}
        }
    },
    {% endfor %}
};

const std::vector<std::pair<std::string, std::vector<std::string>>> rules = {
    {% for rule in grammar %}
    {
        "{{ rule.lhs }}",
        { {% for word in rule.rhs %}"{{ word }}",{% endfor %} }
    },
    {% endfor %}
};

auto scan(InputStack& in)
{
    auto [token, lexeme] = match_longest<ALL_RECOGNIZERS>(in);
    {% if config and config.parser_ignores %}
    {% for ignore in config.parser_ignores %}
    if (token == "{{ ignore }}") {
        return scan(in);
    }
    {% endfor %}
    {% endif %}
    if (std::string(token) == std::string({{ grammar.empty }})) {
        uint32_t a = in.get();
        uint32_t eof = std::char_traits<char>::eof();
        if (a != eof) {
            in.unget(a);
            return std::make_pair("error", lexeme);
        }
    }
    return std::make_pair(token, lexeme);
}

struct SyntaxError {}

struct BaseCallback {
    typedef void (*ShiftHandler)(BaseCallback*, const std::pair<std::string, std::string>&);
    typedef void (*ReduceHandler)(BaseCallback*, const std::pair<std::string, std::vector<std::string>>&);

    std::map<std::string, std::vector<ShiftHandler>> shift_handlers;
    std::map<std::string, std::vector<ReduceHandler>> reduce_handlers;

    void shift(const std::string& token, ShiftHandler handler)
    {
        shift_handlers[token].push_back(handler);
    }

    void reduce(const std::string& rule, ReduceHandler handler)
    {
        reduce_handlers[rule].push_back(handler);
    }

    bool handle_shift(const std::pair<std::string, std::string>& lookahead)
    {
        auto it = shift_handlers.find(lookahead.first);
        if (it == shift_handlers.end()) {
            return false;
        }
        for (auto f: it->second) {
            if (bool ok = f(this, lookahead); !ok) {
                return false;
            }
        }
        return true;
    }

    void handle_reduce(const std::pair<std::string, std::vector<std::string>>& rule)
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
            if (bool ok = f(this, rule); !ok) {
                return false;
            }
        }
        return true;
    }
};

bool {% if config and config.parser_name %}{{ config.parser_name }}{% else %}parse{% endif %}(
        std::istream& is = std::cin,
        BaseCallback* cb = nullptr)
{
    InputStack input_stack(is);
    std::vector<int> states{0};
    std::pair<std::string, std::string> lookahead = scan(input_stack);
    std::unique_ptr<BaseCallback> cb_ptr;
    if (!cb) {
        cb_ptr = std::make_unique<BaseCallback>();
        cb = cb_ptr.get();
    }
    for (;;) {
        auto action = table[states.back()][lookahead.first];
        switch (action.first) {
        case Action::ACCEPT:
            return true;
        case Action::SHIFT:
            cb->handle_shift(lookahead);
            lookahead = scan(input_stack);
            states.push_back(action.second);
            break;
        case Action::REDUCE:
            const auto& rule = rules[action.second];
            cb->handle_reduce(rule);
            for (int i = 0; i < rule.second.size(); ++i) {
                states.pop_back();
            }
            states.push_back(table[states.back()][rule.first].second);
            break;
        default:
            return false;
        }
    }
}

bool {% if config and config.parser_name %}{{ config.parser_name }}{% else %}parse{% endif %}(
        const std::string& s,
        BaseCallback* cb = nullptr)
{
    std::stringstream ss;
    ss << s;
    return {% if config and config.parser_name %}{{ config.parser_name }}{% else %}parse{% endif %}(ss, cb);
}

{% if config and config.parser_namespace %}
};
{% endif %}
