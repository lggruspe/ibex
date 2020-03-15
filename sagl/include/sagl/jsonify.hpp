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

static inline std::string jsonify_automaton(const Table& t)
{
    /* format (example):
     * "automaton": [
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
    std::vector<std::string> entries;
    for (const auto& [state, actions]: t.table) {
        for (const auto& [symbol, action]: actions) {
            std::string entry = R"(
            {
                "state": @state@,
                "symbol": "@symbol@",
                "terminal": @terminal@,
                "type": "@type@",
                "action": @action@
            })";
            find_and_replace(entry, "@state@", std::to_string(state));
            find_and_replace(entry, "@terminal@", t.is_terminal(symbol) ? "true" : "false");
            find_and_replace(entry, "@type@",
                    action.first == Action::ERROR ? "error" :
                    action.first == Action::ACCEPT ? "accept" :
                    action.first == Action::GOTO ? "goto" :
                    action.first == Action::REDUCE ? "reduce" :
                    action.first == Action::SHIFT ? "shift"
                        : "error");
            find_and_replace(entry, "@action@",
                    action.first == Action::ERROR ? "0" : std::to_string(action.second));
            // @symbol@ is replaced last, because symbol might contain
            // @...@, which could affect the other substitutions
            find_and_replace(entry, "@symbol@", symbol);
            entries.push_back(entry);
        }
    }
    std::string automaton = "[\n";
    if (!entries.empty()) {
        automaton += entries[0];
        for (int i = 1; i < entries.size(); ++i) {
            automaton += ",\n" + entries[i];
        }
    }
    automaton += "\n]";
    return automaton;
}

static inline std::string join(
        const std::vector<std::string>& s,
        const std::string& sep = "")
{
    std::string t;
    if (!s.empty()) {
        t += s[0];
    }
    for (int i = 1; i < s.size(); ++i) {
        t += sep + s[i];
    }
    return t;
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
    find_and_replace(json, "@rhs@", "[\"" + join(rhs, "\", \"") + "\"]");
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
        "automaton": @automaton@,
        "grammar": @grammar@
    })";
    find_and_replace(json, "@grammar@", jsonify_grammar(t));
    find_and_replace(json, "@automaton@", jsonify_automaton(t));
    return json;
}
