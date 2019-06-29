#include "grammar.h"
#include "item.h"
#include "state.h"
#include "automaton.h"
#include "parsingtable.h"
#include "parser.h"
#include "scanner.h"
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
    bool success = parse(scan);
    std::cout << success << std::endl;
}
