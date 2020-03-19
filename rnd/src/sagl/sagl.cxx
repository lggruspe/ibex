#include "sagl.h"
#include "sagl.hpp"
#include "parser/parser.hpp"
#include "parser/eval.hpp"
#include <cstdlib>
#include <cstring>
#include <string>

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

char const *sagl_open(char const* bnf)
{
    auto json = parse_grammar(bnf);
    return strdup(json.c_str());
}

void sagl_close(void* m)
{
    free(m);
}
