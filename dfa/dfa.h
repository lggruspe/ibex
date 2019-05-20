#pragma once
#include "nfa.h"
#include <map>
#include <set>
#include <string>

namespace automata 
{
struct Dfa {
    int start;
    std::set<int> accept;
    std::map<int, std::map<std::string, int> > delta;
    std::set<std::string> symbols;

    int add_state(int q)
    {
        this->delta[q];
        return q;
    }

    int add_state()
    {
        int q = this->delta.empty() ? 0 : this->delta.rbegin()->first + 1;
        return this->add_state(q);
    }

    void add_transition(int q, const std::string& a, int r)
    {
        this->add_state(q);
        this->add_state(r);
        this->delta[q][a] = r;
        if (!a.empty()) {
            this->symbols.insert(a);
        }
    }
};

Dfa subset_construction(const Nfa&);

} // end namespace
