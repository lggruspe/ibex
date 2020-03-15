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

static inline std::string jsonify(const Table& t)
{
    /* format (example):
     * {
     *      "automaton": [
     *          {
     *              "state": 0,
     *              "symbol": "A",
     *              "terminal": false,
     *              "type": "reduce",
     *              "action": 5
     *          },
     *          ...
     *      ],
     *      "grammar": [
     *          {
     *              "id": 4,
     *              "lhs": "A",
     *              "rhs": ["a", "A", "b"]
     *          },
     *          ...
     *      ]
     * }
     */
    std::vector<std::string> entries;
    for (const auto& [state, actions]: t.table) {
        for (const auto& [symbol, action]: actions) {
            std::string entry = R"VOGON(
            {
                "state": @state@,
                "symbol": "@symbol@",
                "terminal": @terminal@,
                "type": "@type@",
                "action": @action@
            })VOGON";
            find_and_replace(entry, "@state@", std::to_string(state));
            find_and_replace(entry, "@terminal@", t.grammar_ptr->is_terminal(symbol) ? "true" : "false");
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
    std::string json = R"VOGON(
    {
        "automaton": @automaton@,
        "grammar": @grammar@
    }
    )VOGON";
    std::string automaton = "[\n";
    if (!entries.empty()) {
        automaton += entries[0];
        for (int i = 1; i < entries.size(); ++i) {
            automaton += ",\n" + entries[i];
        }
    }
    automaton += "\n]";
    find_and_replace(json, "@automaton@", automaton);

    // jsonify grammar rules
    std::string grammar_json;
    for (const auto& [rule, id]: t.rules) {
        std::string rhs = "[";
        for (const auto& sym: rule.second) {
            rhs += "\"";
            rhs += sym;
            rhs += "\"";
            rhs += ", ";
        }
        if (rhs.back() != '[') {
            rhs.pop_back();
            rhs.pop_back();
        }
        rhs += ']';

        std::string entry = R"(
        {
            "id": @id@,
            "lhs": @lhs@,
            "rhs": @rhs@
        }
        )";
        find_and_replace(entry, "@id@", std::to_string(id));
        find_and_replace(entry, "@rhs@", rhs);
        find_and_replace(entry, "@lhs@", "\"" + std::string(rule.first) + "\"");
        if (!grammar_json.empty()) {
            grammar_json += ",\n";
        }
        grammar_json += entry;
    }
    grammar_json = "[" + grammar_json + "]";
    find_and_replace(json, "@grammar@", grammar_json);
    return json;
}
