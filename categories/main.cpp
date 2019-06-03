//#include <unicode/unistr.h>
//#include <unicode/chariter.h>
//#include <unicode/schriter.h>
#include "cats.h"
#include "regex2.h"
#include "utilities.h"
#include <iostream>
#include <vector>
#include <list>

using namespace regex2;

void test_alternate()
{
    std::list<Expr> list;
    for (char c = 'a'; c != 'f'; ++c) {
        list.push_back(symbol(c));
    }

    for (const auto& re: list) {
        std::cout << re << ' ';
    }
    std::cout << std::endl;
    auto expr = _alternate(list);
    std::cout << "expr: " << expr << std::endl;
    std::cout << "alphabet: " << *(expr->alphabet) << std::endl;

    std::cout << "---------------------" << std::endl;
}

bool test_categorize()
{
    Alphabet alphabet;
    insert(alphabet, 'a', 'z');
    insert(alphabet, 'A', 'Z');
    insert(alphabet, '0', '9');
    std::cout << alphabet << std::endl;

    std::vector<char> test = {'f', 'H', 'o', '3', ' ', '!'};
    std::vector<int> res = {'a', 'A', 'a', '0', -1, -1};
    for (auto i = 0; i < test.size(); ++i) {
        int cat = categorize(alphabet, test[i]);
        if (cat != res[i]) {
            return false;
        }
    }
    return true;
}

void test_print()
{
    auto X = symbol('0', '6');
    auto Y = symbol('4', '9');
    std::cout << *((X | Y)->alphabet) << std::endl;
    std::cout << (X | Y) << std::endl;
}

int main()
{
    auto re = integer();
    std::cout << "integer" << std::endl;
    std::cout << re << std::endl;
    std::cout << *(re->alphabet) << std::endl;

    re = number();
    std::cout << "number" << std::endl;
    std::cout << re << std::endl;
    std::cout << *(re->alphabet) << std::endl;

    re = identifier();
    std::cout << "identifier" << std::endl;
    std::cout << re << std::endl;
    std::cout << *(re->alphabet) << std::endl;
}
