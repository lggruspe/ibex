#pragma once
#include "nfa.h"
#include <iostream>
#include <map>
#include <set>

namespace automata 
{
struct Dfa {
    int start;
    std::set<int> accept;
    std::map<int, std::map<char, int>> delta;
    std::set<char> symbols;
};

Dfa subset_construction(const Nfa&);
std::ostream& operator<<(std::ostream&, const Dfa&);
Dfa minimize(const Dfa&);

} // end namespace
