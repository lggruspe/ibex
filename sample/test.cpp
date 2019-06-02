#include "grammar.h"
#include "lr1/parser.h"
#include "lr1/item.h"
#include "lr1/collection.h"
#include "print.h"
#include "test.h"
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

int main()
{
    cfg::Grammar<std::string> grammar;
    grammar.add_variable("start");
    grammar.add_variable("literal");
    grammar.add_terminal("float");
    grammar.add_terminal("integer");
    grammar.add_terminal("identifier");
    grammar.add_terminal("");
    grammar.add_rule("start", {"literal"});
    grammar.add_rule("literal", {"float"});
    grammar.add_rule("literal", {"integer"});
    grammar.add_rule("literal", {"identifier"});

    lr1::Parser<std::string> parser(grammar);
    parser.construct("start");

    ScannerCollection scanners;
    integerScanner inscanner("integer");
    floatScanner flscanner("float");
    identifierScanner idscanner("identifier");
    whitespaceScanner wsscanner("");
    scanners.add_scanner(flscanner);
    scanners.add_scanner(inscanner);
    scanners.add_scanner(idscanner);
    scanners.add_scanner(wsscanner);
    bool passed = parser.parse(scanners);
    std::cout << passed << std::endl;
}
