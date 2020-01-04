#include "regexp.h"
#include "../parse/eval.hpp"
#include "../parse/parser.hpp"
#include "../../sagl/include/sagl.hpp"
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <string>

using namespace scanner;

Grammar<Variable, Token> g({
    {Variable::START,   {Variable::EXPR}},
    {Variable::EXPR,    {Variable::EXPR, Token::PIPE, Variable::TERM}},
    {Variable::EXPR,    {Variable::TERM}},
    {Variable::TERM,    {Variable::TERM, Variable::FACTOR}},
    {Variable::TERM,    {Variable::FACTOR}},
    {Variable::FACTOR,  {Variable::VALUE, Token::STAR}},
    {Variable::FACTOR,  {Variable::VALUE, Token::PLUS}},
    {Variable::FACTOR,  {Variable::VALUE, Token::QUESTION}},
    {Variable::FACTOR,  {Variable::VALUE}},
    {Variable::VALUE,   {Token::LPAREN, Variable::EXPR, Token::RPAREN}},
    {Variable::VALUE,   {Token::DOT}},
    {Variable::VALUE,   {Token::SYMBOL}},
    {Variable::VALUE,   {Token::INTERVAL}},
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

void regexp_close(char const* re)
{
    free((void*)re);
}
