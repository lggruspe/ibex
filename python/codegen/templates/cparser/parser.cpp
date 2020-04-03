extern "C" {
    #include "scanner.h"
}
#include "parser.h"
#include <array>
#include <map>
#include <set>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

const std::set<std::string> ignores = {
    "whitespace",
};

enum class Action { ERROR = 0, GOTO, SHIFT, REDUCE, ACCEPT };

std::map<int, std::map<std::string, std::pair<Action, int>>> parse_table {
    {%- for action in table %}
    {
        {{ loop.index0 }}, {
            {%- for entry in table[loop.index0] %}
            {"{{ entry.symbol }}", {Action::{{ entry.type|upper }}, {{ entry.value }}}},
            {%- endfor %}
        }
    },
    {%- endfor %}
};

constexpr std::array<std::tuple<char const*, char const*, int>, {{ grammar|length }}> rules {
    {%- for rule in grammar %}
    std::make_tuple("{{ rule.lhs }}", "{% for word in rule.rhs %}{{ word }}{% if not loop.last %} {% endif %}{% endfor %}", {{ rule.rhs|length }}),
    {%- endfor %}
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
    bool (*reduce)(void*, char const*, char const*, int),
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
            if (reduce && !reduce(arg, std::get<0>(rule), std::get<1>(rule), std::get<2>(rule))) {
                return false;
            }
            for (int i = 0; i < std::get<2>(rule); ++i) {
                states.pop_back();
            }
            states.push_back(parse_table[states.back()][std::get<0>(rule)].second);
            break;
        }
        default:
            return false;
        }
    }
}
