#pragma once
#include "zsymbols.hpp"
#include "automaton.hpp"
#include "utils/handles.hpp"

namespace rnd {

// Only used for testing automaton.hpp
struct Minimized {
    HandleSet<std::set<ZRange>> symbols;
    std::map<int, std::map<int, int>> states;
    std::set<int> accepts;
    int error;

    Minimized(const Automaton& m);
};

}
