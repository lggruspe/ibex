#include "sagl.hpp"
#include "scanner.hpp"
#include <iostream>

std::ostream& operator<<(std::ostream& os, Item item)
{
    os << "{" << item.rule.first << " ->";
    for (int i = 0; i < (int)(item.rule.second.size()); ++i) {
        if (i == item.position) {
            os << " .";
        }
        os << " " << item.rule.second[i];
    }
    if (item.is_complete()) {
        os << " .";
    }
    return os << ", " << item.lookahead << "}";
}

std::ostream& operator<<(
    std::ostream& os,
    const Automaton<Grammar>::State& state)
{
    for (const auto& item: state) {
        os << item << std::endl;
    }
    return os;
}

std::ostream& operator<<(
    std::ostream& os,
    const Automaton<Grammar>& m)
{
    for (const auto& [state, id]: m) {
        os << "CC" << id << ":" << std::endl;
        for (const auto& item: state) {
            os << item << std::endl;
        }
        os << std::endl;
    }
    return os;
}

#include "parser.hpp"
#include "parsetree.hpp"

int main()
{
    Grammar g({
        {"S", {"A"}},
        {"A", {"a", "A", "b"}},
        {"A", {}},
    });
    Parser p(g);
    ParseTreeCallback<decltype(g)::Symbol> callback;
    try {
        auto root = std::move(p.parse(callback));
        std::cout << root << std::endl;
    } catch (SyntaxError e) {
        std::cout << e.what() << std::endl;
    }
}
