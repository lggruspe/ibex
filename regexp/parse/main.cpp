#include "../scan/scanner.hpp"
#include "sagl.hpp"
#include "variable.hpp"
#include <iostream>

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

#include "../../rnd/src/nexpr.hpp"

std::ostream& operator<<(std::ostream& os, const NExpr& expr)
{
    for (const auto& [q, dq]: expr.states) {
        os << "q = " << q << std::endl;
        for (const auto& [a, R]: dq) {
            os << "[" << a.start << ", " << a.end << "): ";
            for (const auto& r: R) {
                os << r << " ";
            }
            os << std::endl;
        }
    }
    return os;
}

#include "parser.hpp"
#include "ast.hpp"

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
    value -> lparen expr rparen
    value -> dot
    value -> symbol
    value -> interval
    */
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
    ast::Callback<decltype(g)::Symbol> cb;
    Parser p(g);
    std::cout << p.parse(cb) << std::endl;
}
