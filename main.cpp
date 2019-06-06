#include "grammar.h"
#include "lr1/parser.h"
#include "lr1/item.h"
#include "lr1/collection.h"
#include "print.h"
#include <algorithm>
#include <cctype>
#include <memory>
#include <iostream>
#include <string>
#include <utility>

/*0 goal -> list
 *1 list -> list pair
 *2 list -> pair
 *3 pair -> ( pair )
 *4 pair -> ()
 */

class Scanner {
    std::string stream;
public:
    Scanner(const char* stream) : stream(stream) {}
    std::pair<std::string,std::string> operator()() {
        auto it = std::find_if(stream.begin(), stream.end(), [](char c) {
                    return !isspace(c);
                });
        auto jt = std::find_if(it, stream.end(), [](char c) {
                    return isspace(c);
                });
        std::string ret(it, jt);
        stream.erase(stream.begin(), jt);
        return std::pair<std::string, std::string>(ret, ret);
    }
};

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


    lr1::Parser<std::string> parser(grammar);
    parser.construct("goal");

    /*
    lr1::print_automaton(parser);
    lr1::print_collections(parser);
    lr1::print_table(parser);
    */

    bool passed = parser.parse(Scanner("( ( ( ) ) ) ( )"));
    std::cout << passed << std::endl;
}
