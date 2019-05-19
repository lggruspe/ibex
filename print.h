#pragma once
#include "lr1.h"
#include <iostream>

namespace lr1
{

void print_item(const Item& item)
{
    std::cout << "{" << item.lhs << " -> ";
    for (const auto& sym: item.before) {
        std::cout << sym << ' ';
    }
    std::cout << ".";
    for (const auto& sym: item.after) {
        std::cout << sym << ' ';
    }
    std::cout << ", " << item.lookahead << "}" << std::endl;
}

void print_state(const Collection& state)
{
    for (const auto& item: state) {
        print_item(item);
    }
}

void print_sentence(const cfg::Sentence& sent)
{
    for (const auto& sym: sent) {
        std::cout << sym << ' ';
    }
    std::cout << std::endl;
}

void print_collections(const Parser& parser)
{
    std::cout << "Parser states" << std::endl;
    for (const auto& state: parser.names) {
        std::cout << state.first << std::endl;
        print_state(state.second);
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void print_automaton(const Parser& parser)
{
    std::cout << "Parser automaton" << std::endl;
    const std::map<int, std::map<cfg::Symbol, int>>& delta = parser.delta;

    for (const auto &transitions: delta) {
        for (const auto& sym: transitions.second) {
            std::cout << "(" << transitions.first << ", " << sym.first 
                << ") = " << sym.second << std::endl;
        }
    }
    std::cout << std::endl;
}

void print_table(const Parser& parser)
{
    std::cout << "Parser table" << std::endl;
    for (const auto& p: parser.table) {
        for (const auto& q: p.second) {
            std::cout << "(" << p.first << ", ";
            std::cout << q.first << ") = "
                << q.second.first << ' ' << q.second.second
                << std::endl;
        }
    }
    std::cout << std::endl;
}

} // end namespace
