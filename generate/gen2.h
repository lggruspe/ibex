#pragma once
#include "cats.h"
#include "dfa.h"
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

namespace gen2
{
;

void includes(std::ostream& out, bool header=false)
{
    if (header) {
        out << "#pragma once" << std::endl;
    }

    out << R"VOGON(#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>
)VOGON" << std::endl;
}

void base_scanner(std::ostream& out)
{
    out << R"VOGON(struct Scanner {
    Token token;
    Scanner(Token token) : token(token) {}
    virtual std::string operator()(std::istream&) = 0;
};
)VOGON" << std::endl;
}

void _range(std::ostream& out, char symbol, const Alphabet& alphabet)
{
    // find interval in alphabet that contains symbol
    auto it = alphabet.find(symbol);
    if (it == alphabet.end()) {
        throw std::invalid_argument("symbol not found in any character range");
    }
    auto low = it->lower();
    auto high = it->upper();
    // TODO check if endpoints are open
    // also check if low = high
    // TODO print character representation (need to deal with special cases:
    // whitespace, escape characters, etc.)
    out << "(" << (int)low << " <= c && c <= " << (int)high << ")";
}

void _category_condition(std::ostream& out,
        int destination, const std::set<char>& symbols,
        const Alphabet& alphabet)
{
    _range(out, *(symbols.begin()), alphabet);
    for (auto it = std::next(symbols.begin()); it != symbols.end(); ++it) {
        out << " || ";
        _range(out, *it, alphabet);
    }
}

void _category(std::ostream& out, int destination,
        const std::set<char>& symbols, const Alphabet& alphabet)
{
    out << "        if (";
    _category_condition(out, destination, symbols, alphabet);
    out << ") {" << std::endl;
    out << "            goto s" << destination << ";" << std::endl;
    out << "        }" << std::endl;
}

void _categories(std::ostream& out,
        const std::map<char, int>& trans, const Alphabet& alphabet)
{
    // invert trans
    std::map<int, std::set<char>> inverse;
    for (const auto& [a, r]: trans) {
        inverse[r].insert(a);
    }
    for (const auto& [dest, syms]: inverse) {
        _category(out, dest, syms, alphabet);
    }
}

void _scanner_state(std::ostream& out, bool accept, int state, 
        const std::map<char, int>& trans, const Alphabet& alphabet)
{
    out << "    s" << state << ":" << std::endl;
    out << R"VOGON(        in.get(c);
        lexeme += c;)VOGON" << std::endl;

    if (accept) {
        out << "        checkpoint.clear();" << std::endl;
    }
    out << "        checkpoint.push_back(c);" << std::endl;
    _categories(out, trans, alphabet);
    out << "        goto se;" << std::endl;
}


void scanner(std::ostream& out, const std::string& name, const automata::Dfa& dfa,
        const Alphabet& alphabet)
{
    // TODO check if name is a valid identifier
    out << "struct ";
    out << name; 
    out << R"VOGON(Scanner: public Scanner {
    using Scanner::Scanner;
    std::string operator()(std::istream& in)
    {
        char c;
        std::vector<char> checkpoint;
        std::string lexeme;)VOGON" << std::endl;

    out << "        goto s" << dfa.start << ";" << std::endl;

    for (const auto& [state, trans]: dfa.delta) {
        bool accept = dfa.accept.count(state);
        _scanner_state(out, accept, state, trans, alphabet);
    }

    out << R"VOGON(    se:
        while (!checkpoint.empty()) {
            c = checkpoint.back();
            checkpoint.pop_back();
            in.putback(c);
            lexeme.pop_back();
        }
        return lexeme;
    }
};
)VOGON" << std::endl;
}

void scanner_collection(std::ostream& out, const std::vector<std::string>& names)
{
    out << R"VOGON(struct ScannerCollection {
    std::vector<std::shared_ptr<Scanner>> scanners;
    std::istream* in;

    ScannerCollection(std::istream& in=std::cin) : in(&in) 
    {)VOGON" << std::endl;
    
    for (const auto& name: names) {
        out << "        scanners.push_back(std::make_shared<" << name 
            << "Scanner>(Token::" << name << "));" << std::endl;
    }
    
    out << R"VOGON(    }

    std::pair<Token, std::string> operator()()
    {
        for (auto scanner: scanners) {
            std::string lexeme = (*scanner)(*in);
            if (!lexeme.empty()) {
                if (scanner->token != Token::Ignore) {
                    return std::pair<Token, std::string>(scanner->token, lexeme);
                }
                return (*this)();
            }
        }
        int c = in->get();
        if (c == std::char_traits<char>::eof()) {
            in->clear();
            return std::pair<Token, std::string>(Token::Empty, "");
        }
        return std::pair<Token, std::string>(Token::Other, std::string(1, (char)c));
    }

    void scan()
    {
        for (;;) {
            try {
                auto [token, lexeme] = (*this)();
                std::cout << "token: " << token << std::endl;
                std::cout << "lexeme: " << lexeme << std::endl;
                std::cout << std::endl;
            } catch (const char* e) {
                in->ignore(1, '\n');
            }
        }
    }
};
)VOGON" << std::endl;
}

bool _is_valid_token(const std::string& name)
{
    return name != "Other" && name != "Empty";
}

void tokens(std::ostream& out, const std::vector<std::string>& names)
{
    out << R"VOGON(enum class Token {
    Empty,
    Other)VOGON";
    for (const auto& name: names) {
        if (!_is_valid_token(name)) {
            throw std::invalid_argument("invalid (reserved) token name");
        }
        out << ",\n    " << name;
    }
    // make sure Ignore is included
    if (std::find(names.begin(), names.end(), "Ignore") == names.end()) {
        out << ",\n    Ignore";
    }
    out << "\n};\n" << std::endl;
}

void token_printer(std::ostream& out, const std::vector<std::string>& names)
{
    // assumes tokens has been called, and all names are valid
    // notice how the Ignore token is conveniently ignored
    out << R"VOGON(std::ostream& operator<<(std::ostream& out, Token token)
{
    switch(token) {
    case Token::Other:
        return out << "Other";
    case Token::Empty:
        return out << "Empty";
)VOGON";
    for (const auto& name: names) {
        if (name == "Ignore") {
            continue;
        }
        out << "    case Token::" << name << ":" << std::endl;
        out << "        return out << \"" << name << "\";" << std::endl;
    }
    out << R"VOGON(    default:
        return out;
    }
}
)VOGON" << std::endl;
}

} // end namespace
