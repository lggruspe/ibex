#include "generator.h"
#include "gen2.h"
#include <rnd/regex.h>
#include <rnd/nfa.h>
#include <rnd/dfa.h>
#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace gen2
{

automata::Dfa _regex_to_dfa(regex::Expr re)
{
    return automata::Dfa(automata::Nfa(re));
}

void generate(std::ostream& out, 
        const std::map<std::string, regex::Expr>& res)
{
    std::vector<std::string> names;
    std::for_each(res.begin(), res.end(), 
            [&names](std::pair<std::string, regex::Expr> pair) { 
            names.push_back(pair.first); });
    includes(out, true);
    tokens(out, names);
    token_printer(out, names);
    base_scanner(out);
    for (const auto& [name, re]: res) {
        scanner(out, name, _regex_to_dfa(re), *(re->alphabet));
    }
    scanner_collection(out, names);
}

}
