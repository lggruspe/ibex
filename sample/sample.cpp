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
#include <variant>

/*0 goal -> list
 *1 list -> list pair
 *2 list -> pair
 *3 pair -> ( pair )
 *4 pair -> ()
 */

int main()
{
    enum Token {
    enum Variable { Start, Literal };   // Start is reserved, shouldn't appear on the right

    cfg::Grammar<Token, Variable> grammar({
            {Start, Literal},
            {Literal, Number},
            {Literal, Identifier}
            });


    cfg::Grammar<Token, Variable> grammar;
    grammar.add(Number);
    grammar.add(Identifier);
    grammar.add(Literal, Identifier);
    grammar.add(Literal, Number);
    grammar.add(Start, Literal);    // note: rules are added "backwards"

    ScannerCollection scanners;
    lr1::Parser<std::variant<Token, Variable>, std::string> parser(grammar, Start);
    auto parse_tree = parser.parse(scanners);
}
