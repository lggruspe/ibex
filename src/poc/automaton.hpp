#pragma once
#include "handles.hpp"
#include "nexpr.hpp"
#include "zsymbols.hpp"
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

auto epsilon_closures(const NExpr& expr)
{
    std::map<int, std::set<int>> closures;
    std::set<int> queue;
    for (const auto& [q, _]: expr.states) {
        closures[q].insert(q);
        queue.insert(q);
    }
    ZRange eps;
    while (!queue.empty()) {
        int q = *(queue.begin());
        queue.erase(queue.begin());
        for (const auto& [p, dp]: expr.states) {
            if (auto it = dp.find(eps); it == dp.end()) {
                continue;
            } else if (it->second.count(q)) {
                auto before = closures[p].size();
                closures[p].insert(closures[q].begin(), closures[q].end());
                auto after = closures[p].size();
                if (before != after) {
                    queue.insert(p);
                }
            }
        }
    }
    return closures;
}
