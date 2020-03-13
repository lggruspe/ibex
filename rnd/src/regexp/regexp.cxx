#include "regexp.h"
#include "regexp/eval.hpp"
#include "regexp/parser.hpp"
#include "regexp/scanner.hpp"
#include "sagl.hpp"
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <string>

using namespace scanner;

/*
start -> expr
expr -> expr pipe term
expr -> term
term -> term factor
term -> factor
factor -> value star
factor -> value plus
factor -> value question
factor -> value
value -> simple
value -> compound
simple -> dot
simple -> symbol
compound -> lparen expr rparen
compound -> lbracket list rbracket
list -> list element
list -> element
element -> symbol
element -> symbol dash symbol
*/

Grammar<Variable> g({
    {Variable::START,       {Variable::EXPR}},
    {Variable::EXPR,        {Variable::EXPR, "pipe", Variable::TERM}},
    {Variable::EXPR,        {Variable::TERM}},
    {Variable::TERM,        {Variable::TERM, Variable::FACTOR}},
    {Variable::TERM,        {Variable::FACTOR}},
    {Variable::FACTOR,      {Variable::VALUE, "star"}},
    {Variable::FACTOR,      {Variable::VALUE, "plus"}},
    {Variable::FACTOR,      {Variable::VALUE, "question"}},
    {Variable::FACTOR,      {Variable::VALUE}},
    {Variable::VALUE,       {Variable::SIMPLE}},
    {Variable::VALUE,       {Variable::COMPOUND}},
    {Variable::SIMPLE,      {"dot"}},
    {Variable::SIMPLE,      {"symbol"}},
    {Variable::COMPOUND,    {"lparen", Variable::EXPR, "rparen"}},
    {Variable::COMPOUND,    {"lbracket", Variable::LIST, "rbracket"}},
    {Variable::LIST,        {Variable::LIST, Variable::ELEMENT}},
    {Variable::LIST,        {Variable::ELEMENT}},
    {Variable::ELEMENT,     {"symbol"}},
    {Variable::ELEMENT,     {"symbol", "dash", "symbol"}},
});

Parser parser(g);

char const* serialize(const rnd::Automaton& fsm)
{
    std::ostringstream json;
    json << "{" << std::endl;
    json << R"VOGON(    "start": 0,
    "error": )VOGON";
    json << fsm.error << "," << std::endl;
    json << R"VOGON(    "accepts": [)VOGON";

    bool first = true;
    for (const auto& f: fsm.accepts) {
        if (first) {
            first = false;
        } else {
            json << ", ";
        }
        json << f;
    }
    json << "]," << std::endl;

    json << R"VOGON(    "transitions": [)VOGON" << std::endl;
    for (int q = 0; q < (int)(fsm.states.size()); ++q) {
        json << R"VOGON(        [)VOGON" << std::endl;
        bool first = true;
        for (auto it = fsm.states.at(q).begin(); it != fsm.states.at(q).end(); ++it) {
            const auto& [a, r] = *it;
            if (first) {
                first = false;
            } else {
                json << "," << std::endl;
            }
            json << R"VOGON(            [{"start": )VOGON";
            json << a.start << ", \"end\": " << a.end << "}, " << r << "]";
        }
        json << "]";
        if (q != (int)(fsm.states.size()) - 1) {
            json << ",";
        }
        json << std::endl;
    }
    json << R"VOGON(    ])VOGON" << std::endl;
    json << "}";
    return strdup(json.str().c_str());
}

char const* regexp_open(char const* re)
{
    std::string s = re;
    std::istringstream is(s);
    eval::Callback<decltype(g)::Symbol> cb;
    auto m = parser.parse(cb, is);
    return serialize(m);
}

void regexp_close(void* re)
{
    free(re);
}
