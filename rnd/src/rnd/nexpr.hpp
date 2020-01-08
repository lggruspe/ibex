#pragma once
#include "rnd/zsymbols.hpp"
#include <limits>
#include <map>
#include <set>

namespace rnd {

// Represents a regular expression/NFA.
// The start state is always 0 and the accept state is always 1.
struct NExpr {
    ZPartition symbols;
    std::map<int, std::map<ZRange, std::set<int>>> states;

    // Constructs an NFA for the empty set.
    // Note: use the other constructor for epsilon.
    NExpr()
    {
        states[0];
        states[1];
    }

    NExpr(const ZRange& a) : NExpr()
    {
        states[0][a].insert(1);
        symbols.insert(a);
    }
};

NExpr closure(const NExpr&);

NExpr concatenate(const NExpr&, const NExpr&);

NExpr alternate(const NExpr&, const NExpr&);

inline NExpr epsilon()
{
    constexpr auto limit = std::numeric_limits<uint32_t>::max();
    return NExpr(ZRange(limit, limit));
}

}
