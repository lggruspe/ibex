#include "dfa.h"
#include "hopcroft.h"
#include "nfa.h"
#include "regex.h"
#include "utilities.h"
#include "generator.h"
#include <iostream>

using namespace automata;

int main()
{
    regex::Expr re = regex::real();
    Nfa nfa = thompson(re);
    Dfa dfa = minimize(subset_construction(nfa));
    std::map<std::string, std::string> categories;
    categories["zero"] = "0";
    categories["nonzero"] = "123456789";
    categories["exponent"] = "eE";
    categories["sign"] = "+-";
    categories["."] = ".";

    generate_includes(std::cout, true);
    generate_class(std::cout, "float", dfa, categories, 4);
}
