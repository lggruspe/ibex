#pragma once
#include "regex2/regex2.h"
#include <map>
#include <memory>
#include <set>
#include <string>
#include <iostream>

namespace automata 
{

struct Nfa {
    std::map<int, std::map<char, std::set<int>>> delta;
    int start, accept;
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

    void add_transition(int q, int r, char a='\0')
    {
        this->add_state(q);
        this->add_state(r);
        this->delta[q][a].insert(r);
        if (a) {
            this->symbols.insert(a);
        }
    }
};

Nfa thompson(regex2::Expr);

std::ostream& operator<<(std::ostream&, const Nfa&);
} // end namespace
