#include "rnd/minauto.hpp"
#include "rnd/automaton.hpp"
#include "rnd/zsymbols.hpp"
#include <map>
#include <set>
#include <vector>

namespace rnd {

std::vector<int> transition_vector(const Automaton& m, const ZRange& a)
{
    std::vector<int> R;
    for (const auto& [q, dq]: m.states) {
        R.push_back(dq.at(a));
    }
    return R;
}

void categorize_symbol(
        const Automaton& m,
        const ZRange& a,
        std::map<std::vector<int>, std::set<ZRange>>& partition,
        std::map<ZRange, std::vector<int>>& categories)
{
    auto R = transition_vector(m, a);
    partition[R].insert(a);
    categories[a] = R;
}

Minimized::Minimized(const Automaton& m)
    : accepts(m.accepts)
    , error(m.error)
{
    // assume m has minimized rows
    std::map<std::vector<int>, std::set<ZRange>> partition;
    std::map<ZRange, std::vector<int>> categories;
    for (const auto& a: m.symbols.to_set()) {
        categorize_symbol(m, a, partition, categories);
    }
    for (const auto& [_, v]: partition) {
        symbols.insert(v);
    }
    for (const auto& [q, dq]: m.states) {
        for (const auto& [a, r]: dq) {
            auto b = symbols.index(partition[categories[a]]);
            states[q][b] = r;
        }
    }
}

}
