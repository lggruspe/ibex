#pragma once
#include "alphabet.hpp"
#include "nfa.h"
#include <iostream>
#include <map>
#include <set>

namespace automata 
{
struct Dfa {
    int start;
    std::set<int> accept;
    std::map<int, std::map<Alphabet::Category, int>> delta;
    std::set<Alphabet::Category> symbols;

    Dfa() {}
    Dfa(const Nfa&);
};

std::ostream& operator<<(std::ostream&, const Dfa&);

} // end namespace
