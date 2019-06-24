#pragma once
#include "regex2/regex2.h"
#include <iostream>
#include <map>
#include <set>

namespace automata 
{
    
struct Nfa {
    std::map<int, std::map<char, std::set<int>>> delta;
    int start;
    int accept;
    std::set<char> symbols;

    Nfa(char);
    Nfa(regex2::Expr);
};

std::ostream& operator<<(std::ostream&, const Nfa&);
}
