#include "helper.hpp"
#include <iostream>

enum class Variable { S, A };
enum class Terminal { A, B, EMPTY };

std::ostream& operator<<(std::ostream& out, const Variable& v)
{
    switch (v) {
    case Variable::S:
        return out << "S";
    case Variable::A:
        return out << "A";
    default:
        throw 0;
    }
}

std::ostream& operator<<(std::ostream& out, const Terminal& t)
{
    switch (t) {
    case Terminal::A:
        return out << "a";
    case Terminal::B:
        return out << "b";
    case Terminal::EMPTY:
        return out << "EMPTY";
    default:
        throw 0;
    }
}

std::ostream& operator<<(std::ostream& out, const typename Grammar<Variable, Terminal>::Symbol& a)
{
    switch (a.index()) {
    case 0:
        return out << std::get<0>(a);
    case 1:
        return out << std::get<1>(a);
    default:
        throw 0;
    }
}

std::ostream& operator<<(std::ostream& os, Item<Grammar<Variable, Terminal>::Symbol> item)
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
    const Automaton<Grammar<Variable, Terminal>>::State& state)
{
    for (const auto& item: state) {
        os << item << std::endl;
    }
    return os;
}

std::ostream& operator<<(
    std::ostream& os,
    const Automaton<Grammar<Variable, Terminal>>& m)
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

int main()
{
    Grammar<Variable, Terminal> g({
        {Variable::S, {Variable::A}},
        {Variable::A, {Terminal::A, Variable::A, Terminal::B}},
        {Variable::A, {}},
    });

    Automaton m(g);
    std::cout << m << std::endl;
    Table table(g);
}
