#pragma once
#include "zsymbols.hpp"
#include "nexpr.hpp"
#include <map>
#include <set>

struct Automaton {
    // start state = 0
    // TODO explicit error state
    ZPartition symbols;
    std::map<int, std::map<ZRange, int>> states;
    std::set<int> accepts;

    Automaton(const NExpr& expr);
};
