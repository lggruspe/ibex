#include "regexp.h"
#include "../parse/eval.hpp"
#include "../parse/parser.hpp"
#include "../../sagl/include/sagl.hpp"
#include <rnd.h>
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

rnd_dfa serialize(const rnd::Automaton& fsm)
{
    rnd_dfa dfa;
    dfa.order = fsm.states.size();
    dfa.states = new rnd_state[dfa.order];
    dfa.error = fsm.error;
    int i = 0;
    for (const auto& [q, dq]: fsm.states) {
        assert(i == q);
        auto& state = dfa.states[q];
        state.accept = false;
        state.outdegree = dq.size();
        state.transitions = new rnd_transition[state.outdegree];
        auto& trans = state.transitions;
        int j = 0;
        for (const auto& [a, r]: dq) {
            trans[j].label.start = a.start;
            trans[j].label.end = a.end;
            trans[j].to = r;
            ++j;
        }
        ++i;
    }
    for (const auto& q: fsm.accepts) {
        dfa.states[q].accept = true;
    }
    return dfa;
}

rnd_dfa rnd_parse_regex(char const* re)
{
    std::string s = re;
    std::istringstream is(s);
    eval::Callback<decltype(g)::Symbol> cb;
    auto m = parser.parse(cb, is);
    return serialize(m);
}
