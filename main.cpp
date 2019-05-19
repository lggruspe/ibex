#include "grammar.h"
#include "lr1.h"
#include <iostream>

/* list -> list pair
 * list -> pair
 * pair -> ( pair )
 * pair -> ()
 */

int main()
{
    cfg::Grammar grammar;
    grammar.add_variable("list");
    grammar.add_variable("pair");
    grammar.add_terminal("(");
    grammar.add_terminal(")");
    grammar.add_rule("list", {"list", "pair"});
    grammar.add_rule("list", {"pair"});
    grammar.add_rule("pair", {"(", "pair", ")"});
    grammar.add_rule("pair", {"(", ")"});
    grammar.add_variable("goal");
    grammar.start = "goal";
    grammar.add_rule("goal", {"list"});

    //grammar.compute_first_sets();
    //lr1::Parser parser;
    //parser.construct_automaton(grammar);
}
