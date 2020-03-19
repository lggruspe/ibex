#pragma once
#include "zsymbols.hpp"
#include "nexpr.hpp"
#include <map>

namespace rnd {

struct Automaton {
    // start state = 0
    // error state = -1 by default
    // (assumes after minimization, that there's a single error state)
    ZPartition symbols;
    std::map<int, std::map<ZRange, int>> states;
    std::set<int> accepts;
    int error;

    Automaton(const NExpr& expr);
};

}
