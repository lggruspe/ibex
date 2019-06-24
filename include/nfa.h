#pragma once
#include "regex2/regex2.h"
#include <iostream>
#include <map>
#include <set>

namespace automata 
{

struct Nfa {
    std::map<int, std::map<char, std::set<int>>> delta;
    int start, accept;
    std::set<char> symbols;
};

Nfa thompson(regex2::Expr);

std::ostream& operator<<(std::ostream&, const Nfa&);
} // end namespace
