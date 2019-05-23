#pragma once
#include "lr1/item.h"
#include "lr1/parser.h"
#include <iostream>

namespace lr1
{

template <class Symbol>
void print_item(const Item<Symbol>& item)
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

template <class Symbol>
void print_state(const Collection<Symbol>& state)
{
    for (const auto& item: state.items) {
        print_item(item);
    }
}

template <class Symbol>
void print_collections(const Parser<Symbol>& parser)
{
    std::cout << "Parser states" << std::endl;
    for (const auto& state: parser.collections) {
        std::cout << state.first << std::endl;
        print_state(state.second);
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

template <class Symbol>
void print_automaton(const Parser<Symbol>& parser)
{
    std::cout << "Parser automaton" << std::endl;
    const std::map<int, std::map<Symbol, int>>& delta = parser.delta;

    for (const auto &transitions: delta) {
        for (const auto& sym: transitions.second) {
            std::cout << "(" << transitions.first << ", " << sym.first 
                << ") = " << sym.second << std::endl;
        }
    }
    std::cout << std::endl;
}

template <class Symbol>
void print_table(const Parser<Symbol>& parser)
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
