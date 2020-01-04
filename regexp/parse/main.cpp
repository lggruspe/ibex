#include "../scan/scanner.hpp"
#include "sagl.hpp"
#include "variable.hpp"
#include <iostream>
#include <sstream>

std::ostream& operator<<(std::ostream& out, const typename Grammar<Variable, scanner::Token>::Symbol& a)
{
    switch (a.index()) {
    case 0:
        return out << std::get<0>(a);
    case 1:
        return out << std::get<1>(a);
    default:
        throw 0;
    }
}

std::ostream& operator<<(std::ostream& os, Item<Grammar<Variable, scanner::Token>::Symbol> item)
{
    os << "{" << item.rule.first << " ->";
    for (int i = 0; i < (int)(item.rule.second.size()); ++i) {
        if (i == item.position) {
            os << " .";
        }
        os << " " << item.rule.second[i];
    }
    if (item.is_complete()) {
        os << " .";
    }
    return os << ", " << item.lookahead << "}";
}

std::ostream& operator<<(
    std::ostream& os,
    const Automaton<Grammar<Variable, scanner::Token>>::State& state)
{
    for (const auto& item: state) {
        os << item << std::endl;
    }
    return os;
}

std::ostream& operator<<(
    std::ostream& os,
    const Automaton<Grammar<Variable, scanner::Token>>& m)
{
    for (const auto& [state, id]: m) {
        os << "CC" << id << ":" << std::endl;
        for (const auto& item: state) {
            os << item << std::endl;
        }
        os << std::endl;
    }
    return os;
}

#include "../../rnd/src/automaton.hpp"

std::ostream& operator<<(std::ostream& os, const rnd::Automaton& fsm)
{
    for (const auto& [q, dq]: fsm.states) {
        for (const auto& [a, r]: dq) {
            os << "d(" << q << ", [" << a.start << ", " << a.end << ")) = " << r << std::endl;
        }
    }
    os << "accepts: ";
    for (const auto& f: fsm.accepts) {
        os << f << " ";
    }
    os << "\nerror: " << fsm.error;
    return os;
}

#include "parser.hpp"
#include "eval.hpp"

using namespace scanner;

int main()
{
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
    list -> list simple
    list -> simple
    list -> simple dash simple
    */
    Grammar<Variable, Token> g({
        {Variable::START,       {Variable::EXPR}},
        {Variable::EXPR,        {Variable::EXPR, Token::PIPE, Variable::TERM}},
        {Variable::EXPR,        {Variable::TERM}},
        {Variable::TERM,        {Variable::TERM, Variable::FACTOR}},
        {Variable::TERM,        {Variable::FACTOR}},
        {Variable::FACTOR,      {Variable::VALUE, Token::STAR}},
        {Variable::FACTOR,      {Variable::VALUE, Token::PLUS}},
        {Variable::FACTOR,      {Variable::VALUE, Token::QUESTION}},
        {Variable::FACTOR,      {Variable::VALUE}},
        {Variable::VALUE,       {Variable::SIMPLE}},
        {Variable::VALUE,       {Variable::COMPOUND}},
        {Variable::SIMPLE,      {Token::DOT}},
        {Variable::SIMPLE,      {Token::SYMBOL}},
        {Variable::COMPOUND,    {Token::LPAREN, Variable::EXPR, Token::RPAREN}},
        {Variable::COMPOUND,    {Token::LBRACKET, Variable::LIST, Token::RBRACKET}},
        {Variable::LIST,        {Variable::LIST, Variable::SIMPLE}},
        {Variable::LIST,        {Variable::SIMPLE}},
        {Variable::LIST,        {Variable::SIMPLE, Token::DASH, Variable::SIMPLE}},
    });

    eval::Callback<decltype(g)::Symbol> cb;
    Parser p(g);
    std::string s;
    std::cin >> s;
    std::istringstream is(s);
    auto m = p.parse(cb, is);
    std::cout << m << std::endl;
}
