#include "grammar.h"
//#include "target/asdf.h"
#include <set>
#include <iostream>

using namespace cfg;

/*
enum Variable { Start, Literal };

std::ostream& operator<<(std::ostream& out, Variable var)
{
    switch (var) {
    case Start:
        return out << "Start";
    case Literal:
        return out << "Literal";
    }
}

template <class T, class V>
std::ostream& operator<<(std::ostream& out, Symbol<T, V> sym)
{
    if (sym.is_variable()) {
        out << sym.variable();
    } else {
        out << sym.token();
    }
    return out;
}

template <class T>
void print(const T& sentence)
{
    for (const auto& sym: sentence) {
        std::cout << sym << ' ';
    }
    std::cout << std::endl;
}

void test_first()
{
    Grammar<Token, Variable> grammar(Token::Empty);
    grammar.add(Token::Number);
    grammar.add(Token::Identifier);
    grammar.add(Token::Empty);
    grammar.add(Token::Ignore);
    grammar.add(Token::Other);

    // add rule
    grammar.add(Literal, {Token::Identifier});
    grammar.add(Literal, {Token::Number});
    grammar.add(Start, {Literal});

    std::cout << "first(" << Token::Number << "): ";
    print(grammar.first(Token::Number));

    std::cout << "first(" << Token::Identifier << "): ";
    print(grammar.first(Token::Identifier));

    std::cout << "first(" << Token::Empty << "): ";
    print(grammar.first(Token::Empty));

    std::cout << "first(" << Token::Ignore << "): ";
    print(grammar.first(Token::Ignore));

    std::cout << "first(" << Token::Other << "): ";
    print(grammar.first(Token::Other));

    std::cout << "first(" << Literal << "): ";
    print(grammar.first(Literal));

    std::cout << "first(" << Start << "): ";
    print(grammar.first(Start));

}
*/

enum Token { Lparen, Rparen, Empty };

enum Variable { Goal, List, Pair };

template <class T>
void print(const T& first_set)
{
    for (const auto& sym: first_set) {
        if (sym.is_token()) {
            std::cout << "T" << sym.token();
        } else if (sym.is_variable()) {
            std::cout << "V" << sym.variable();
        }
        std::cout << ' ';
    }
    std::cout << std::endl;
}

void test_paren()
{
    Grammar<Token, Variable> grammar(Empty);
    grammar.add(Lparen);
    grammar.add(Rparen);
    grammar.add(Pair, {Lparen, Rparen});
    grammar.add(Pair, {Lparen, Pair, Rparen});
    grammar.add(List, {Pair});
    grammar.add(List, {List, Pair});
    grammar.add(Goal, {List});

    std::cout << Lparen << ": ";
    print(grammar.first(Lparen));
    std::cout << Rparen << ": ";
    print(grammar.first(Rparen));
    std::cout << Empty << ": ";
    print(grammar.first(Empty));

    std::cout << Goal << ": ";
    print(grammar.first(Goal));
    std::cout << List << ": ";
    print(grammar.first(List));
    std::cout << Pair << ": ";
    print(grammar.first(Pair));
}

int main()
{
    test_paren();
}
