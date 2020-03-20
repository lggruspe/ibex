#pragma once
#include "tabulate.hpp"
#include <string>
#include <vector>

static inline void find_and_replace(
        std::string& text,
        const std::string& before,
        const std::string& after)
{
    auto pos = text.find(before);
    if (pos != std::string::npos) {
        text.replace(pos, before.length(), after);
    }
}

static inline std::string join(
        const std::vector<std::string>& s,
        const std::string& sep = "")
{
    std::string t;
    if (!s.empty()) {
        t += s[0];
    }
    for (int i = 1; i < (int)s.size(); ++i) {
        t += sep + s[i];
    }
    return t;
}

static inline std::string jsonify_action(
        int state,
        const std::string& symbol,
        Action type,
        int value)
{
    std::string action = R"(
    {
        "state": @state@,
        "symbol": "@symbol@",
        "type": "@type@",
        "value": @value@
    })";
    find_and_replace(action, "@value@", std::to_string(value));
    find_and_replace(action, "@type@",
            type == Action::GOTO ? "goto" :
            type == Action::SHIFT ? "shift" :
            type == Action::REDUCE ? "reduce" :
            type == Action::ACCEPT ? "accept" :
            "error");
    find_and_replace(action, "@symbol@", symbol);
    find_and_replace(action, "@state@", std::to_string(state));
    return action;
}

static inline std::string jsonify_actions(
        int state,
        const std::map<std::string, std::pair<Action, int>>& state_actions)
{
    std::vector<std::string> actions;
    for (const auto& [symbol, action]: state_actions) {
        actions.push_back(jsonify_action(state, symbol, action.first, action.second));
    }
    return "[" + join(actions, " ,") + "]";
}

static inline std::string jsonify_table(const Table& t)
{
    /* format (example):
     * "table": [
     *     {
     *         "state": 0,
     *         "symbol": "A",
     *         "terminal": false,
     *         "type": "reduce",
     *         "action": 5
     *     },
     *     ...
     * ]
     */
    std::vector<std::string> rows;
    for (const auto& [state, actions]: t.table) {
        rows.push_back(jsonify_actions(state, actions));
    }
    return "[" + join(rows, ",") + "]";
}

static inline std::string jsonify_rhs(const std::vector<std::string>& rhs)
{
    std::vector<std::string> words;
    for (const auto& word: rhs) {
        words.push_back("\"" + word + "\"");
    }
    return "[" + join(words, ", ") + "]";
}

template <class Rule = std::pair<std::string, std::vector<std::string>>>
static inline std::string jsonify_rule(int id, const Rule& rule)
{
    std::string json = R"(
    {
        "id": @id@,
        "lhs": @lhs@,
        "rhs": @rhs@,
        "rule": @rule@
    })";
    const auto& [lhs, rhs] = rule;
    find_and_replace(json, "@rule@", "\"" + lhs + " -> " + join(rhs, " ") + "\"");
    find_and_replace(json, "@lhs@", "\"" + lhs + "\"");
    find_and_replace(json, "@rhs@", jsonify_rhs(rhs));
    find_and_replace(json, "@id@", std::to_string(id));
    return json;
}

static inline std::string jsonify_grammar(const Table& t)
{
    std::map<int, std::string> rules;
    for (const auto& [rule, id]: t.rules) {
        rules[id] = jsonify_rule(id, rule);
    }
    std::vector<std::string> json_rules;
    for (const auto& [_, rule]: rules) {
        json_rules.push_back(rule);
    }
    return "[" + join(json_rules, ", ") + "]";
}

static inline std::string jsonify(const Table& t)
{
    std::string json = R"(
    {
        "table": @table@,
        "grammar": @grammar@
    })";
    find_and_replace(json, "@grammar@", jsonify_grammar(t));
    find_and_replace(json, "@table@", jsonify_table(t));
    return json;
}
