#include "scanner.h"
#include <fstream>
#include <iostream>
#include <string>
#include <utility>

// TODO redefine cfg::Symbol to be std::pair<std::String, std::String>
// or use templates
bool Parser::parse(std::istream& in, cfg::Symbol scan(std::istream&))
{
    std::vector<int> states = {start};
    std::vector<cfg::Symbol> symbols;
    cfg::Symbol lookahead = scan(in);
    while (true) {
        int state = states.back();
        lr1::Action action = table[state][lookahead];
        if (action.first == "accept") {
            return true;
        } else if (action.first == "shift") {
            states.push_back(action.second);
            symbols.push_back(lookahead);
            lookahead = scan(in);
        } else if (action.first == "reduce") {
            cfg::Rule rule = grammar.rule(action.second);
            for (auto i = 0; i < rule.second.size(); ++i) {
                states.pop_back();
                symbols.pop_back();
            }
            symbols.push_back(rule.first);
            states.push_back(table[states.back()][rule.first]);
        } else {
            return false;
        }
    }
}

int main()
{
    Parser parser("goal -> line\n"
            "list -> list pair\n"
            "list -> pair\n"
            "pair -> ( pair )\n"
            "pair -> ( )\n");

    std::ifstream file("asdf.cpp");
    auto tree = parser.parse(file);
}
