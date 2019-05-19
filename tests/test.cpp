#include "grammar.h"
#include "lr1.h"
#include <iostream>
#include <vector>

bool test_closures()
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


    lr1::Collection start = {
        lr1::Item(grammar.start, {""},
                *(grammar.productions(grammar.start).begin()), "")
    };
    lr1::take_closure(grammar, start);
    std::cout << start.size() << std::endl;
    return start.size() == 9;
}

int main()
{
    bool passed = test_closures();
    std::cout << passed << std::endl;
}
