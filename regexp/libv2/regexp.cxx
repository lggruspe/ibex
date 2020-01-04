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
    std::string json = R"VOGON({
    "start": 0,
    "accepts": $ACCEPTS,
    "transitions": $TRANSITIONS,
    "error": $ERROR,
})VOGON";

    std::ostringstream error;
    error << fsm.error;
    json.replace(json.find("$ERROR"), 6, error.str());

    std::ostringstream accepts;
    accepts << "[";
    for (auto it = fsm.accepts.begin(); it != fsm.accepts.end(); ++it) {
        if (it == fsm.accepts.begin()) {
            accepts << *it;
        } else {
            accepts << ", " << *it;
        }
    }
    accepts << "]";
    json.replace(json.find("$ACCEPTS"), 8, accepts.str());

    std::ostringstream transitions;
    transitions << "[";
    for (int q = 0; q < (int)(fsm.states.size()); ++q) {
        if (q != 0) {
            transitions << ", ";
        }
        transitions << "[";
        bool first = true;
        for (const auto& [a, r]: fsm.states.at(q)) {
            if (!first) {
                transitions << ", ";
            }
            transitions << "[{\"start\": " << a.start << ", \"end\": " << a.end;
            transitions << "}, " << r << "]";
        }
        transitions << "]";
    }
    transitions << "]";
    json.replace(json.find("$TRANSITIONS"), 12, transitions.str());
    return strdup(json.c_str());
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
