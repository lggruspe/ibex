#include "grammar.h"
#include "parser.h"
#include "scanner.h"
#include <algorithm>
#include <iostream>
#include <variant>
#include <vector>

enum class Variable { start, list };

std::ostream& operator<<(std::ostream& out, Variable v)
{
    switch (v) {
    case Variable::start:
        return out << "start";
    case Variable::list:
        return out << "list";
    }
    return out;
}

using Symbol = std::variant<Token, Variable>;

std::ostream& operator<<(std::ostream& out, Symbol sym)
{
    switch (sym.index()) {
    case 0:
        return out << std::get<0>(sym);
    case 1:
        return out << std::get<1>(sym);
    }
    return out;
}

std::vector<Symbol> symbols;
int counter = 0;

int accept_cb(bool accept)
{
    return accept ? counter : -counter;
}

template <class Rule>
void reduce_cb(Rule rule)
{
    std::for_each(rule.rhs.begin(), rule.rhs.end(),
            [](const auto&) {
            symbols.pop_back();
            });
    symbols.push_back(rule.lhs);
    counter += (rule.rhs.size() - 1);
}

template <class TokenLexemePair>
void shift_cb(TokenLexemePair lookahead)
{
    symbols.push_back(lookahead.first);
    ++counter;
}

int main()
{
    sagl::Grammar<Symbol>
    grammar(Variable::start, Token::_empty, {
            {Variable::start, {Variable::list}},
            {Variable::list, {Token::a, Variable::list}},
            {Variable::list, {Token::a}}
            });
    sagl::Parser parse(grammar);

    ScannerCollection scan;
    auto success = parse(scan, accept_cb, reduce_cb, shift_cb);
    std::cout << "success = " << success << std::endl;
    std::cout << "counter = " << counter << std::endl;
}
