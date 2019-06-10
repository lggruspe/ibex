#include "asdf.h"
#include "grammar.h"
#include "lr1/parser.h"
#include <iostream>

int main()
{
    ScannerCollection scanner;

    enum class Variable { start, list, literal };

    cfg::Grammar<Token, Variable> grammar(Token::_empty);
    // TODO what about _ignore and _empty?
    grammar.add(Token::_empty);
    grammar.add(Token::character);
    grammar.add(Token::identifier);
    grammar.add(Token::number);
    grammar.add(Token::string);

    grammar.add(Variable::literal, {Token::character});
    grammar.add(Variable::literal, {Token::identifier});
    grammar.add(Variable::literal, {Token::number});
    grammar.add(Variable::literal, {Token::string});
    grammar.add(Variable::list, {Variable::literal});
    grammar.add(Variable::list, {Variable::literal, Variable::list});
    grammar.add(Variable::start, {Variable::list});

    lr1::Parser parser(grammar);
    parser.construct(Variable::start);
    bool success = parser.parse(scanner);
    std::cout << success << std::endl;
}
