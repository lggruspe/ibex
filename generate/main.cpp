#include "dfa.h"
#include "hopcroft.h"
#include "nfa.h"
#include "regex.h"
#include "utilities.h"
#include "generator.h"
#include <iostream>

using namespace automata;

Dfa regex_to_dfa(regex::Expr re)
{
    Nfa nfa = thompson(re);
    return minimize(subset_construction(nfa));
}

void generate_whitespace_scanner()
{
    Dfa dfa = regex_to_dfa(regex::whitespace());
    std::map<std::string, std::string> categories;
    categories[""] = "\\n \\t";  // take note of the double slash
    generate_scanner_class(std::cout, "whitespace", dfa, categories, 4);
}

void generate_float_scanner()
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
    generate_scanner_class(std::cout, "float", dfa, categories, 4);
}

void generate_integer_scanner()
{
    regex::Expr re = regex::integer();
    Nfa nfa = thompson(re);
    Dfa dfa = minimize(subset_construction(nfa));

    std::map<std::string, std::string> categories;
    categories["zero"] = "0";
    categories["nonzero"] = "123456789";
    generate_scanner_class(std::cout, "integer", dfa, categories, 4);
}

void generate_identifier_scanner()
{
    regex::Expr re = regex::identifier();
    Nfa nfa = thompson(re);
    Dfa dfa = minimize(subset_construction(nfa));

    std::map<std::string, std::string> categories;
    categories["letter"] = "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    categories["digit"] = "1234567890";
    generate_scanner_class(std::cout, "identifier", dfa, categories, 4);
}

int main()
{
    generate_includes(std::cout, true);
    generate_base_scanner_class(std::cout, 4);

    generate_whitespace_scanner();
    generate_integer_scanner();
    generate_float_scanner();
    generate_identifier_scanner();

    generate_collection_class(std::cout, 4);
}
