#pragma once
#include "rnd/alphabet/alphabet.hpp"
#include "rnd/nfa/nfa.h"
#include <iostream>
#include <map>
#include <set>

namespace rnd
{
namespace dfa 
{
struct Dfa {
    int start;
    std::set<int> accept;
    std::map<int, std::map<SymbolRange, int>> delta;
    std::set<SymbolRange> symbols;

    Dfa() {}
    Dfa(const nfa::Nfa&);
    Dfa(regex::Expr);
};

std::ostream& operator<<(std::ostream&, const Dfa&);

} // end namespace
}
