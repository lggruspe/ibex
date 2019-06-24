#include "generate/generator.h"
#include "regex2/regex2.h"
#include "dfa.h"
#include "nfa.h"
#include "gen2.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace gen2
{

automata::Dfa _regex_to_dfa(regex2::Expr re)
{
    auto nfa(re);
    return automata::minimize(automata::subset_construction(nfa));
}

void generate(std::ostream& out, 
        const std::map<std::string, regex2::Expr>& res)
{
    std::vector<std::string> names;
    std::for_each(res.begin(), res.end(), 
            [&names](std::pair<std::string, regex2::Expr> pair) { 
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
