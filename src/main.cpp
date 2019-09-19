#include "scanners.hpp"
#include "match.hpp"
#include "sagl.hpp"
#include <algorithm>
#include <iostream>
#include <variant>
#include <vector>

enum class Variable { start, list };

std::ostream& operator<<(std::ostream& out, Variable v)
{
    switch (v) {
    case Variable::start:
        return out << "start";
    case Variable::list:
        return out << "list";
    }
    return out;
}

using Symbol = std::variant<Token, Variable>;

std::ostream& operator<<(std::ostream& out, Symbol sym)
{
    switch (sym.index()) {
    case 0:
        return out << std::get<0>(sym);
    case 1:
        return out << std::get<1>(sym);
    }
    return out;
}

std::vector<Symbol> symbols;
int counter = 0;

int accept_cb(bool accept)
{
    return accept ? counter : -counter;
}

template <class Rule>
void reduce_cb(Rule rule)
{
    const auto& [symbol, sentence] = rule;
    std::for_each(sentence.begin(), sentence.end(),
            [](const auto&) {
                symbols.pop_back();
            });
    symbols.push_back(symbol);
    counter += sentence.size() - 1;
}

template <class TokenLexemePair>
void shift_cb(TokenLexemePair lookahead)
{
    symbols.push_back(lookahead.first);
    ++counter;
}

struct Scan {
    match::Tokenizer<SCAN_ALL> tokenizer;

    std::pair<Token, std::string> operator()()
    {
        return tokenizer.tokenize();
    }
};

int main()
{
    sagl::Grammar<Symbol>
    grammar(Variable::start, Token::empty, {
            {Variable::start, {Variable::list}},
            {Variable::list, {Token::a, Variable::list}},
            {Variable::list, {Token::a}}
            });
    sagl::ParseTable parse_table(grammar);
    auto success = sagl::parse<Symbol>(Scan(), parse_table, grammar,
            accept_cb, reduce_cb, shift_cb);
    std::cout << "success = " << success << std::endl;
    std::cout << "counter = " << counter << std::endl;
}
