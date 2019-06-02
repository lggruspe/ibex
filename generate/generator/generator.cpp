#include "generator.h"
#include "dfa.h"
#include <iostream>
#include <map>
#include <set>
#include <string>

void indented(std::ostream& out, const std::string& s,
        std::string::size_type indent=4)
{
    out << std::string(indent, ' ') << s << std::endl;
}

void generate_includes(std::ostream& out, bool header=false)
{
    if (header) {
        out << "#pragma once" << std::endl;
    }

    out << R"VOGON(#include <iostream>
#include <string>
#include <utility>
#include <vector>
)VOGON" << std::endl;
}


void generate_base_scanner_class(std::ostream& out, std::string::size_type indent=4)
{
    out << "class Scanner {" << std::endl;
    out << "protected:" << std::endl;
    indented(out, "bool contains(const char* s, char c)", indent);
    indented(out, "{", indent);
    indented(out, "auto it = s;", 2*indent);
    indented(out, "while (*it && *it != c) {", 2*indent);
    indented(out, "++it;", 3*indent);
    indented(out, "}", 2*indent);
    indented(out, "return *it == c;", 2*indent);
    indented(out, "}", indent);
    out << std::endl;

    out << "public:" << std::endl;
    indented(out, "std::string token;", indent);
    indented(out, "Scanner(const std::string& token) : token(token) {}", indent);
    indented(out, "virtual std::string operator()(std::istream&) = 0;", indent);
    out << "};\n" << std::endl;
}

void generate_collection_class(std::ostream& out, std::string::size_type indent=4)
{
    out << "class ScannerCollection {" << std::endl;
    indented(out, "std::vector<Scanner*> scanners;", indent);
    indented(out, "std::istream* in;", indent);

    out << "\npublic:" << std::endl;
    indented(out, "ScannerCollection(std::istream& in=std::cin) : in(&in) {};", indent);

    // void add_scanner(Scanner&)
    indented(out, "void add_scanner(Scanner& scanner)", indent);
    indented(out, "{", indent);
    indented(out, "scanners.push_back(&scanner);", 2*indent);
    indented(out, "}", indent);
    out << std::endl;

    // operator()
    indented(out, "std::pair<std::string, std::string> operator()()", indent);
    indented(out, "{", indent);
    indented(out, "for (auto& p: scanners) {", 2*indent);

    indented(out, "Scanner& scanner = *p;", 3*indent);
    indented(out, "std::string lexeme = scanner(*in);", 3*indent);
    indented(out, "if (!lexeme.empty()) {", 3*indent);

    indented(out, "if (!scanner.token.empty()) {", 4*indent);
    indented(out, "return std::pair<std::string, std::string>(scanner.token, lexeme);", 5*indent);
    indented(out, "}", 4*indent);
    indented(out, "return (*this)();", 4*indent);

    indented(out, "}", 3*indent);

    indented(out, "}", 2*indent);
    indented(out, "if (in->eof()) {", 2*indent);

    indented(out, "return std::pair<std::string, std::string>(\"\", \"\");", 3*indent);

    indented(out, "}", 2*indent);

    indented(out, "int c = in->get();", 2*indent);
    indented(out, "if (c == std::char_traits<char>::eof()) {", 2*indent);
    indented(out, "in->clear();", 3*indent);
    indented(out, "return std::pair<std::string, std::string>(\"\", \"\");", 3*indent);
    indented(out, "}", 2*indent);
    indented(out, "return std::pair<std::string, std::string>(\"other\", std::string(1, (char)c));", 2*indent);
    indented(out, "}", indent);
    out << std::endl;

    // void scan();
    indented(out, "void scan()", indent);
    indented(out, "{", indent);
    indented(out, "for (;;) {", 2*indent);
    indented(out, "try {", 3*indent);
    indented(out, "auto [token, lexeme] = (*this)();", 4*indent);
    indented(out, "std::cout << \"token: \" << token << std::endl;", 4*indent);
    indented(out, "std::cout << \"lexeme: \" << lexeme << std::endl;", 4*indent);
    indented(out, "std::cout << std::endl;", 4*indent);
    indented(out, "} catch (const char* e) {", 3*indent);
    indented(out, "in->ignore(1, '\\n');", 4*indent);
    indented(out, "}", 3*indent);
    indented(out, "}", 2*indent);
    indented(out, "}", indent);
    out << "};\n" << std::endl;
}

void generate_categories(std::ostream& out,
        const std::map<std::string, std::string>& categories,
        std::string::size_type indent=4)
{
    indented(out, "std::string category(char c)", indent);
    indented(out, "{", indent);
    for (const auto& cat: categories) {
        indented(out, "if (contains(\"" + cat.second + "\", c)) {", 
                2*indent);
        indented(out, "return \"" + cat.first + "\";", 3*indent);
        indented(out, "}", 2*indent);
    }

    indented(out, "return \"other\";", 2*indent);
    indented(out, "}", indent);
    out << std::endl;
}

void generate_scanner_state_transitions(std::ostream& out,
    const std::map<std::string, int>& transitions,
    std::string::size_type indent=4)
{
    // invert transitions
    std::map<int, std::set<std::string>> dests;
    for (const auto& [sym, dest]: transitions) {
        dests[dest].insert(sym);
    }
    for (const auto& [dest, syms]: dests) {
        std::string cond = "cat == \"" + *(syms.begin()) + "\"";
        for (auto it = std::next(syms.begin()); it != syms.end(); ++it) {
            cond += " || cat == \"" + *it + "\"";
        }
        indented(out, "if (" + cond + ") {", 2*indent);
        indented(out, std::string("goto s") + (char)(dest + '0') + ";", 3*indent);
        indented(out, "}", 2*indent);
    }
}

void generate_scanner_state(std::ostream& out, const automata::Dfa& dfa,
    int state, std::string::size_type indent=4)
{
    indented(out, std::string("s") + (char)(state + '0') + ":", indent);
    indented(out, "in.get(c);", 2*indent);
    indented(out, "lexeme += c;", 2*indent);
    if (dfa.accept.count(state)) {
        indented(out, "checkpoint.clear();", 2*indent);
    }
    indented(out, "checkpoint.push_back(c);", 2*indent);
    indented(out, "cat = category(c);", 2*indent);

    generate_scanner_state_transitions(out, dfa.delta.at(state), indent);
    indented(out, "goto se;", 2*indent);
    out << std::endl;
}

void generate_scanner_states(std::ostream& out, const automata::Dfa& dfa,
    std::string::size_type indent=4)
{
    for (const auto& state: dfa.delta) {
        generate_scanner_state(out, dfa, state.first, indent);
    }
}

void generate_scanner_rollback(std::ostream& out,
    std::string::size_type indent=4)
{
    indented(out, "se:", indent);
    indented(out, "while (!checkpoint.empty()) {", 2*indent);
    indented(out, "c = checkpoint.back();", 3*indent);
    indented(out, "checkpoint.pop_back();", 3*indent);
    indented(out, "in.putback(c);", 3*indent);
    indented(out, "lexeme.pop_back();", 3*indent);
    indented(out, "}", 2*indent);
}

void generate_scanner(std::ostream& out, const std::string& token,
    const automata::Dfa& dfa, std::string::size_type indent=4)
{

    indented(out, "std::string operator()(std::istream& in)", indent);
    indented(out, "{", indent);

    indented(out, "char c;", 2*indent);
    indented(out, "std::string cat;", 2*indent);
    indented(out, "std::vector<char> checkpoint;", 2*indent);
    indented(out, "std::string lexeme;", 2*indent);
    indented(out, std::string("goto s") + (char)(dfa.start + '0') + ";", 2*indent);

    generate_scanner_states(out, dfa, indent);
    generate_scanner_rollback(out, indent);

    indented(out, "return lexeme;", 2*indent);
    indented(out, "}", indent);
}

void generate_scanner_class(std::ostream& out, const std::string& token, 
        const automata::Dfa& dfa, 
        const std::map<std::string, std::string>& categories,
        std::string::size_type indent=4)
{
    out << "class " + token + "Scanner: public Scanner {" << std::endl;
    generate_categories(out, categories, indent);

    out << "public:" << std::endl;
    indented(out, "using Scanner::Scanner;", indent);

    generate_scanner(out, token, dfa, indent);
    out << "};\n" << std::endl;
}
