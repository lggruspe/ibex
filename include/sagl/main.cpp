#include "grammar.h"
#include "item.h"
#include "state.h"
#include "automaton.h"
#include "parsingtable.h"
#include "parser.h"
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

    for (const auto& [q, T]: automaton.transitions) {
        for (const auto& [a, r]: T) {
            std::cout << "d(" << q << ", " << a << ") = " << r << std::endl;
        }
    }
}

void test_parsing_table()
{
    sagl::Grammar<Symbol>
    grammar(Variable::start, Terminal::empty, {
            {Variable::start, {Variable::list}},
            {Variable::list, {Terminal::a, Variable::list}},
            {Variable::list, {Terminal::a}}
            });

    sagl::Automaton automaton(grammar);
    sagl::ParsingTable parser(automaton, grammar);

    for (const auto& [state_id, actions]: parser.table) {
        for (const auto& [symbol, action]: actions) {
            std::cout << "action(" << state_id << ", " << symbol << ") = " << action << std::endl;
        }
    }
}

void test_temp()
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

    sagl::Rule<Symbol> rule1(Variable::list, {Terminal::a});
    sagl::Rule<Symbol> rule2(Variable::list, {Terminal::a, Variable::list});

    auto r1 = grammar.rule_index(rule1);
    auto r2 = grammar.rule_index(rule2);

    sagl::Item<Symbol> item3(r1, rule1, Terminal::empty);
    sagl::Item<Symbol> item4(r2, rule2, Terminal::empty);
    auto item1 = item3.shifted();
    auto item2 = item4.shifted();

    sagl::State state({item1, item2, item3, item4}, grammar);
    std::cout << state << std::endl;

    auto transitions = state.transitions(grammar);
    for (const auto& [symbol, next_state]: transitions) {
        std::cout << "transition(" << symbol << "):" << std::endl;
        std::cout << next_state << std::endl;
        std::cout << std::endl;
    }
}

void test_parser()
{
sagl::Grammar<Symbol>
    grammar(Variable::start, Terminal::empty, {
            {Variable::start, {Variable::list}},
            {Variable::list, {Terminal::a, Variable::list}},
            {Variable::list, {Terminal::a}}
            });
    sagl::Parser parse(grammar);
}

int main()
{
    test_parser();
}
