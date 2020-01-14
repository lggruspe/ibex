#pragma once
#include "rnd/zsymbols.hpp"
#include "rnd/automaton.hpp"
#include "handles.hpp"

namespace rnd {


struct MinimizedAutomaton {
    HandleSet<std::set<ZRange>> symbols;
    std::map<int, std::map<int, int>> states;
    std::set<int> accepts;
    int error;

    MinimizedAutomaton(const Automaton& m);
};

}
