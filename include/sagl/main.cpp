#include "grammar.h"
#include "item.h"
#include "state.h"
#include "automaton.h"
#include <iostream>
#include <variant>
#include <vector>

enum class Terminal { empty, a };
enum class Variable { start, list };

std::ostream& operator<<(std::ostream& out, Terminal t)
{
    switch (t) {
    case Terminal::empty:
        return out << "empty";
    case Terminal::a:
        return out << "a";
    }
}

std::ostream& operator<<(std::ostream& out, Variable v)
{
    switch (v) {
    case Variable::start:
        return out << "start";
    case Variable::list:
        return out << "list";
    }
}

using Symbol = std::variant<Terminal, Variable>;

std::ostream& operator<<(std::ostream& out, Symbol sym)
{
    switch (sym.index()) {
    case 0:
        return out << std::get<0>(sym);
    case 1:
        return out << std::get<1>(sym);
    }
    return out;
}

void test_grammar()
{
    sagl::Grammar<Symbol>
    grammar(Variable::start, Terminal::empty, {
            {Variable::start, {Variable::list}},
            {Variable::list, {Terminal::a, Variable::list}},
            {Variable::list, {Terminal::a}}
            });

    // print symbols
    std::cout << "symbols: ";
    for (const auto& sym: grammar.symbols) {
        std::cout << sym << " ";
    }
    std::cout << std::endl;
    std::cout << "start: " << grammar.start << std::endl;
    std::cout << "empty: " << grammar.empty << std::endl;

    // print rules
    std::cout << "rules:" << std::endl;
    auto [start, end] = grammar.rules_for(grammar.start);

    for (auto it = start; it != end; ++it) {
        const auto& lhs = it->lhs;
        const auto& rhs = it->rhs;
        std::cout << lhs << " -> ";
        for (const auto& sym: rhs) {
            std::cout << sym << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    // print first
    for (const auto& sym: grammar.symbols) {
        std::cout << "first(" << sym << ") = {";
        std::vector<Symbol> sentence = {sym};
        for (const auto& first: grammar.first(sentence)) {
            std::cout << first << " ";
        }
        std::cout << "}" << std::endl;
    }

}

void test_state()
{
    sagl::Grammar<Symbol>
    grammar(Variable::start, Terminal::empty, {
            {Variable::start, {Variable::list}},
            {Variable::list, {Terminal::a, Variable::list}},
            {Variable::list, {Terminal::a}}
            });

    sagl::Item<Symbol> item(
            grammar.rule_index({Variable::start, {Variable::list}}),
            {Variable::start, {Variable::list}},
            Terminal::empty);

    sagl::State state({item}, grammar);
}

void test_automaton()
{
    sagl::Grammar<Symbol>
    grammar(Variable::start, Terminal::empty, {
            {Variable::start, {Variable::list}},
            {Variable::list, {Terminal::a, Variable::list}},
            {Variable::list, {Terminal::a}}
            });

    sagl::Automaton automaton(grammar);

    /*
    std::cout << "automaton has " << automaton.transitions.size() << " transitions" << std::endl;
    for (const auto& [q, T]: automaton.transitions) {
        for (const auto& [a, r]: T) {
            std::cout << "d(" << q << ", " << a << ") = " << r << std::endl;
        }
    }
    */
}

int main()
{
    test_automaton();
}
