#include "regex.h"
#include "nfa.h"
#include "dfa.h"
#include <string>

using namespace regex;
using namespace automata;

Expr empty()
{
    return symbol('\0');
}

Expr optional(Expr a)
{
    return empty() | a;
}

Expr integer()
{
    auto nonzero = symbol('1', '9');
    auto digit = symbol('0', '9');
    return symbol('0') | (nonzero + closure(digit));
}

Expr number()
{
    auto digit = symbol('0', '9');
    auto decimal = symbol('.') + digit + closure(digit);

    auto exponent = (symbol('e') | symbol('E'))
        + optional(symbol('-') | symbol('+'))
        + integer();
        
    return integer() + optional(decimal) + optional(exponent);
}

Expr identifier()
{
    auto letter = symbol('_') | symbol('a', 'z') | symbol('A', 'Z');
    return letter + closure(letter | symbol('0', '9'));
}

Expr whitespace()
{
    return symbol(' ') | symbol('\t') | symbol('\n');
}

Expr character()
{
    Expr escape = symbol('\\') + (symbol('\'') | symbol('\\') | symbol('t') | symbol('n'));
    Expr middle = symbol(32, 38) | symbol(40, 91) | symbol(93, 126) | escape;
    return symbol('\'') + middle + symbol('\'');
}

Expr string()
{
    return symbol('"') + closure((symbol(32, 33) | symbol(35, 91) | symbol(93, 126))
            | (symbol('\\') + symbol(32, 126))) + symbol('"');
}

void print_regex_to_dfa(const std::string& label, Expr expr)
{
    std::cout << label << std::endl;
    std::cout << expr << std::endl;
    std::cout << Dfa(Nfa(expr)) << std::endl;
}

int main()
{
    std::cout << identifier() << std::endl;
    //print_regex_to_dfa("identifier", identifier());
    //print_regex_to_dfa("number", number());
    //print_regex_to_dfa("character", character());
    //print_regex_to_dfa("string", string());
}
