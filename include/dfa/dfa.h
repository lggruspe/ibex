#pragma once
#include "nfa.h"
#include <iostream>
#include <map>
#include <set>

namespace automata 
{
struct Dfa {
    int start;
    std::set<int> accept;
    std::map<int, std::map<char, int> > delta;
    std::set<char> symbols;

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

    void add_transition(int q, int r, char a)
    {
        this->add_state(q);
        this->add_state(r);
        this->delta[q][a] = r;
        if (a) {
            this->symbols.insert(a);
        }
    }
};

Dfa subset_construction(const Nfa&);
std::ostream& operator<<(std::ostream&, const Dfa&);

} // end namespace
