#pragma once
#include "automaton.h"
#include "grammar.h"
#include "parsingtable.h"
#include <map>

namespace sagl
{
;

template <class Symbol>
class Parser {
    std::map<int, std::map<Symbol, Action>> table;

public:
    Parser(const Grammar<Symbol>& grammar)
    {
        Automaton automaton(grammar);
        ParsingTable parsing_table(automaton, grammar);
        table = parsing_table.table;
    }

    // TODO pass shift, reduce, accept and goto callbacks
    template <class Scanner>
    bool parse(Scanner scan) const;
};

}
