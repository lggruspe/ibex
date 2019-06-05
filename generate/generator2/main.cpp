#include "dfa.h"
#include "hopcroft.h"
#include "nfa.h"
#include "_gen2.h"
#include "regex2.h"
#include "utilities.h"
#include <iostream>

using namespace regex2;
using namespace automata;
using namespace gen2;

void regex_to_dfa(Expr re)
{
    auto nfa = thompson(re);
    return minimize(subset_construction(nfa));
}

void whitespace_scanner()
{
    auto re = whitespace();
    auto dfa = regex_to_dfa(re);
    scanner(std::cout, "whitespace", dfa, re->alphabet);
}

void number_scanner()
{
    auto re = number();
    auto dfa = regex_to_dfa(re);
    scanner(std::cout, "number", dfa, re->alphabet);
}

void identifier_scanner()
{
    auto re = identifier();
    auto dfa = regex_to_dfa(re);
    scanner(std::cout, "identifier", dfa, re->alphabet);
}

int main()
{
    includes(std::cout, true);
    base_scanner(std::cout);
    whitespace_scanner();
    number_scanner();
    identifier_scanner();
    scanner_collection(std::cout);
}
