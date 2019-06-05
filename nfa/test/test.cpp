#include "_nfa.h"
#include "nfa.h"
#include "utilities.h"
#include <iostream>
#include <boost/icl/split_interval_set.hpp>

using namespace automata;

void test_thompson_empty()
{
    std::cout << "test_thompson_empty" << std::endl;
    auto re = regex2::empty();
    auto nfa = thompson(re);
    std::cout << nfa << std::endl;
}

void test_thompson_symbol()
{
    std::cout << "test_thompson_symbol (d)" << std::endl;
    auto re = regex2::symbol('d');
    auto nfa = thompson(re);
    std::cout << nfa << std::endl;
}

void test_thompson_union()
{
    std::cout << "test_thompson_union (a | b)" << std::endl;
    auto re = regex2::symbol('a') | regex2::symbol('b');
    auto nfa = thompson(re);
    std::cout << nfa << std::endl;
}

void test_nfa_alternate()
{
    std::cout << "test_nfa_alternate (a | b)" << std::endl;
    auto rea = regex2::symbol('a');
    auto nfaa = thompson(rea);
    std::cout << "nfaa" << std::endl;
    std::cout << nfaa << std::endl;
    auto reb = regex2::symbol('b');
    auto nfab = thompson(reb);
    std::cout << "nfab" << std::endl;
    std::cout << nfab << std::endl;
    auto nfa = alternate(nfaa, nfab);
    std::cout << "nfa" << std::endl;
    std::cout << nfa << std::endl;
}

void test_nfa_closure()
{
    std::cout << "test_nfa_closure (a*)" << std::endl;
    auto rea = regex2::symbol('a');
    auto nfaa = thompson(rea);
    std::cout << "nfaa" << std::endl;
    std::cout << nfaa << std::endl;

    auto nfa = closure(nfaa);
    std::cout << "nfa" << std::endl;
    std::cout << nfa << std::endl;
}

void test_nfa_concatenate()
{
    std::cout << "test_nfa_concatenate (a + b)" << std::endl;
    auto rea = regex2::symbol('a');
    auto nfaa = thompson(rea);
    std::cout << "nfaa" << std::endl;
    std::cout << nfaa << std::endl;
    auto reb = regex2::symbol('b');
    auto nfab = thompson(reb);
    std::cout << "nfab" << std::endl;
    std::cout << nfab << std::endl;

    auto nfa = concatenate(nfaa, nfab);
    std::cout << "nfa" << std::endl;
    std::cout << nfa << std::endl;
}

int main()
{
    regex2::Expr intr = regex2::integer();
    regex2::Expr re = regex2::number();
    regex2::Expr id = regex2::identifier();

    std::cout << "Integer: " << intr << std::endl;
    Nfa nfa = thompson(intr);
    std::cout << nfa << std::endl;

    std::cout << "Number: " << re << std::endl;
    nfa = thompson(re);
    std::cout << nfa << std::endl;

    std::cout << "Identifier: " << id << std::endl;
    nfa = thompson(id);
    std::cout << nfa << std::endl;
}
