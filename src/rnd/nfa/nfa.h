#pragma once
#include "rnd/alphabet/alphabet.hpp"
#include "rnd/regex/regex.h"
#include <iostream>
#include <map>
#include <set>

namespace rnd
{
namespace nfa 
{
    
struct Nfa {
    std::map<int, std::map<SymbolRange, std::set<int>>> delta;
    int start;
    int accept;
    std::set<SymbolRange> symbols;

    Nfa(SymbolRange);
    Nfa(regex::Expr);
};

std::ostream& operator<<(std::ostream&, const Nfa&);
}
}
