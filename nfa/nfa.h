#pragma once
#include "regex.h"
#include <map>
#include <memory>
#include <set>
#include <string>

namespace automata 
{

struct Nfa {
    std::map<int, std::map<std::string, std::set<int>>> delta;
    int start, accept;
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
        this->delta[q][a].insert(r);
        if (!a.empty()) {
            this->symbols.insert(a);
        }
    }
};

Nfa thompson(std::weak_ptr<regex::Tree>);

} // end namespace
