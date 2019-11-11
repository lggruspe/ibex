#pragma once
#include "handles.hpp"
#include "nexpr.hpp"
#include "zsymbols.hpp"
#include <cassert>
#include <iterator>
#include <map>
#include <set>

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

auto next_state(
    const NExpr& expr,
    const std::set<int>& Q,
    const ZRange& a,
    const std::map<int, std::set<int>>& closures)
{
    std::set<int> R;
    for (int q: Q) {
        const auto& dq = expr.states.at(q);
        auto it = dq.find(a);
        if (it == dq.end()) {
            continue;
        }
        for (const auto& r: it->second) {
            R.insert(closures.at(r).begin(), closures.at(r).end());
        }
    }
    return R;
}

void subset_construction(Automaton* m, const NExpr& expr)
{
    // m must point to initialized memory
    auto closures = epsilon_closures(expr);
    HandleSet<std::set<int>> states;
    std::set<int> queue = {states.index(closures[0])};
    while (!queue.empty()) {
        int qid = *(queue.begin());
        queue.erase(queue.begin());
        auto q = states.value(qid);
        for (const auto& a: expr.symbols) {
            auto r = next_state(expr, q, a, closures);
            auto [rid, ok] = states.insert(r);
            m->states[qid][a] = rid;
            if (ok) {
                queue.insert(rid);
            }
        }
        if (q.count(1)) {
            m->accepts.insert(qid);
        }
    }
    m->symbols = expr.symbols;
}

struct PartitionRefiner {
    PartitionRefiner(Automaton* m_) : m_(m_)
    {
        // initial partition: accept 1, else 0
        for (const auto& [q, _]: m_->states) {
            set(q, m_->accepts.count(q));
        }
        refine();
    }

private:
    Automaton* m_;
    std::vector<std::set<int>> cls;
    std::map<int, int> rep;

    void set(int item, int i)
    {
        assert(i <= (int)(cls.size()));
        if (auto it = rep.find(item); it != rep.end()) {
            cls[it->second].erase(item);
        }
        rep[item] = i;
        if (i < (int)(cls.size())) {
            cls[i].insert(item);
        } else if (i == (int)(cls.size())) {
            cls.push_back({item});
        }
    }

    int next_index() const
    {
        return cls.size();
    }

    bool split(int index, const ZRange& a)
    {
        const auto& c = cls[index];
        std::map<int, std::set<int>> destinations;
        for (int q: c) {
            int r = m_->states.at(q).at(a);
            destinations[rep[r]].insert(q);
        }
        if (destinations.size() == 1) {
            return false;
        }
        auto it = std::next(destinations.begin());
        while (it != destinations.end()) {
            int i = next_index();
            for (int q: it->second) {
                set(q, i);
            }
            ++it;
        }
        return true;
    }

    bool split(int index)
    {
        bool changed = false;
        for (const auto& a: m_->symbols) {
            changed = changed || split(index, a);
        }
        return changed;
    }

    bool single_pass_refine()
    {
        bool changed = false;
        for (int i = 0; i < (int)(cls.size()); ++i) {
            changed = changed || split(i);
        }
        return changed;
    }

    void refine()
    {
        while (single_pass_refine())
            ;
    }
};

void minimize(Automaton* m)
{ 
    PartitionRefiner p(m);
    // TODO copy results back to m
}

void set_error_state(Automaton* m)
{
    m->error = -1;
    // TODO find sink state, assuming m is minimized
}

Automaton::Automaton(const NExpr& expr)
{
    subset_construction(this, expr);
    minimize(this);
    set_error_state(this);
}
