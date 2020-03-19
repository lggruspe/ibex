#include "grammar_parser.h"
#include "parser.hpp"
#include "parsetree.hpp"
#include "sagl/sagl.hpp"
#include <string>

using namespace scanner;

std::string parse_grammar(const std::string& s)
{
    // can throw SyntaxError
    ParseTreeCallback cb;
    bool ok = parser::parse(s, &cb);
    auto [start, rules] = cb.accept(ok);
    Grammar g(start, rules);
    Table t(g);
    return jsonify(t);
}
