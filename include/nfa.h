#pragma once
#include "regex.h"
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
    Nfa(regex::Expr);
};

std::ostream& operator<<(std::ostream&, const Nfa&);
}
