#pragma once
#include "../dfa/dfa.h"
#include <fstream>
#include <iostream>
#include <map>
#include <string>

void generate_category(std::ofstream& out, const automata::Dfa& dfa, const std::map<std::string, 
        std::string>& categories)
{
    for (auto it = categories.begin(); it != categories.end(); ++it) {
        out << 
            "\tif (contains(\""
            << it->second << "\", c)) {\n"
            << "\t\treturn \"" << it->first << "\";\n"
            "\t}\n";
    }
    out << "\treturn \"other\";\n";
}

void generate(std::ofstream& out, const automata::Dfa& dfa, const std::map<std::string, 
        std::string>& categories)
{
    out <<
        "#pragma once\n"
        "#include <string>\n"
        << std::endl;

    out <<
        "bool contains(const char *s, char c)\n"
        "{\n"
        "\tauto it = s;\n"
        "\twhile (*it && *it != c) {\n"
        "\t\t++it;\n"
        "\t}\n"
        "\treturn *it == c;\n"
        "}\n"
        << std::endl;

    out <<
        "std::string category(char c)\n"
        "{\n";

    generate_category(out,dfa, categories);

    out << "}\n" << std::endl;
}
