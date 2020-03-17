#include "grammar_parser.h"
#include "parser.hpp"
#include "parsetree.hpp"
#include "sagl.hpp"
#include <sstream>
#include <string>

using namespace scanner;

Grammar grammar("Grammar", {
    {"Grammar", {"Rules"}},
    {"Rules",   {"Rules", "Rule"}},
    {"Rules",   {}},
    {"Rule",    {"Lhs", "arrow", "Rhs", "dot"}},
    {"Lhs",     {"identifier"}},
    {"Rhs",     {"Rhs", "identifier"}},
    {"Rhs",     {}},
});
Parser parser(grammar);

std::string parse_grammar(const std::string& s)
{
    // can throw SyntaxError
    std::stringstream ss;
    ss << s;
    ParseTreeCallback cb;
    auto [start, rules] = parser.parse(cb, ss);
    Grammar g(start, rules);
    Table t(g);
    return jsonify(t);
}
