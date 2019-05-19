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
    grammar.add_terminal("");
    grammar.add_rule("list", {"list", "pair"});
    grammar.add_rule("list", {"pair"});
    grammar.add_rule("pair", {"(", "pair", ")"});
    grammar.add_rule("pair", {"(", ")"});
    grammar.add_variable("goal");
    grammar.start = "goal";
    grammar.add_rule("goal", {"list"});

    lr1::Parser parser(grammar);

    /*
    for (const auto& rule: grammar.rules) {
        std::cout << rule.first << ' ' << rule.second.first << ": ";
        for (const auto& sym: rule.second.second) {
            std::cout << sym << ' ';
        }
        std::cout << std::endl;
    }
    */

    for (const auto& p: parser.table) {
        std::cout << "(" << p.first << ", ";
        for (const auto& q: p.second) {
            std::cout << q.first << ") = "
                << q.second.first << ' ' << q.second.second
                << std::endl;
        }
    }
}
