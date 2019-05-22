#include "grammar.h"
#include "lr1/parser.h"
#include "lr1/item.h"
#include "lr1/collection.h"
//#include "print.h"
#include <memory>
#include <iostream>
#include <string>

/*0 goal -> list
 *1 list -> list pair
 *2 list -> pair
 *3 pair -> ( pair )
 *4 pair -> ()
 */

int main()
{
    cfg::Grammar<std::string> grammar;
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

    for (const auto& sym: grammar.first("goal")) {
        std::cout << sym << ' ';
    }
    std::cout << std::endl;

    lr1::Item<std::string> item("goal", {""}, {"list"}, "");
    lr1::Item<std::string> jtem("goal", {""}, {"list"}, "");

    lr1::Collection<std::string> coll;
    coll.items.insert(item);
    coll.closure(grammar);

    for (const auto& item: coll.items) {
        std::cout << "{" << item.lhs << " -> ";
        for (const auto& sym: item.before) {
            std::cout << sym << ' ';
        }
        std::cout << " .";
        for (const auto& sym: item.after) {
            std::cout << ' ' << sym;
        }
        std::cout << ", " << item.lookahead << "}" << std::endl;
    }
    std::cout << std::endl;

    /*
    lr1::Parser parser(grammar);
    parser.construct("goal");
    */

    /*
    lr1::print_automaton(parser);
    lr1::print_collections(parser);
    lr1::print_table(parser);
    */
}
