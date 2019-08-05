#pragma once
#include "alphabet.hpp"
#include "regex.h"
#include <iostream>
#include <map>
#include <set>

namespace automata 
{
    
struct Nfa {
    std::map<int, std::map<Alphabet::Category, std::set<int>>> delta;
    int start;
    int accept;
    std::set<Alphabet::Category> symbols;

    Nfa(Alphabet::Category);
    Nfa(regex::Expr);
};

std::ostream& operator<<(std::ostream&, const Nfa&);
}
