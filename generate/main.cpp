#include "generate.h"
#include "generate_category.h"
#include "../dfa/dfa.h"
#include "../regex/utilities.h"
#include "../regex/regex.h"
#include "../nfa/nfa.h"
#include "../nfa/thompson.h"
#include "../dfa/hopcroft.h"

#include <fstream>

using namespace automata;

int main(int argc, char **argv)
{
    if (argc < 2) {
        std::cerr << "missing operand\n";
        return 1;
    }

    regex::Re re = regex::real();
    Nfa nfa = thompson(re);
    Dfa dfa = minimize(subset_construction(nfa));
    
    std::ofstream out;
    out.open(argv[1]);

    generate(out, "scanner", dfa);
    extra(out, "scanner");
    out.close();


    std::map<std::string, std::string> categories;
    /*
    categories["letter"] = "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    categories["digit"] = "1234567890";     // TODO do this with regex (regex.categories() to get table)
    */
    categories["zero"] = "0";
    categories["nonzero"] = "123456789";
    categories["exponent"] = "eE";
    categories["sign"] = "+-";
    categories["."] = ".";
    /*
    categories["zero"] = "0";
    categories["nonzero"] = "123456789";
    */

    out.open("category.h");
    generate(out, dfa, categories);
    out.close();
}
