#include "grammar.h"
#include "lr1.h"
#include "print.h"
#include <iostream>

/*0 goal -> list
 *1 list -> list pair
 *2 list -> pair
 *3 pair -> ( pair )
 *4 pair -> ()
 */

int main()
{
    cfg::Grammar grammar;
    grammar.add_variable("goal");
    grammar.add_variable("list");
    grammar.add_variable("pair");
    grammar.add_terminal("(");
    grammar.add_terminal(")");
    grammar.add_terminal("");
    grammar.add_rule("goal", {"list"});
    grammar.add_rule("list", {"list", "pair"});
    grammar.add_rule("list", {"pair"});
    grammar.add_rule("pair", {"(", "pair", ")"});
    grammar.add_rule("pair", {"(", ")"});
    grammar.start = "goal";

    lr1::Parser parser(grammar);

    lr1::print_automaton(parser);
    lr1::print_collections(parser);
    lr1::print_table(parser);
}
