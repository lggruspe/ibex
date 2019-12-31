#include "sagl.hpp"
#include "scanner.hpp"
#include <iostream>

enum class Variable { S, A, ERROR };

std::ostream& operator<<(std::ostream& out, const Variable& v)
{
    switch (v) {
    case Variable::S:
        return out << "S";
    case Variable::A:
        return out << "A";
    default:
        throw 0;
    }
}

std::ostream& operator<<(std::ostream& out, const typename Grammar<Variable, Token>::Symbol& a)
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

std::ostream& operator<<(std::ostream& os, Item<Grammar<Variable, Token>::Symbol> item)
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
    const Automaton<Grammar<Variable, Token>>::State& state)
{
    for (const auto& item: state) {
        os << item << std::endl;
    }
    return os;
}

std::ostream& operator<<(
    std::ostream& os,
    const Automaton<Grammar<Variable, Token>>& m)
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

#include "parser.hpp"
#include "parsetree.hpp"

int main()
{
    Grammar<Variable, Token> g({
        {Variable::S, {Variable::A}},
        {Variable::A, {Token::A, Variable::A, Token::B}},
        {Variable::A, {}},
    });
    /*
    sagl::Parser p(R"VOGON(
        S -> A
        A -> a A b
        A ->
        a = a
        b = b 
    )VOGON");
    */
    Parser p(g);
    ParseTreeCallback<decltype(g)::Symbol> callback;
    try {
        auto root = std::move(p.parse(callback));
        std::cout << root << std::endl;
    } catch (SyntaxError e) {
        std::cout << e.what() << std::endl;
    }
}
