//#include <unicode/unistr.h>
//#include <unicode/chariter.h>
//#include <unicode/schriter.h>
#include "cats.h"
#include "regex2.h"
#include <iostream>
#include <vector>

using namespace regex2;

bool test_categorize()
{
    Alphabet alphabet;
    alphabet.insert('a', 'z');
    alphabet.insert('A', 'Z');
    alphabet.insert('0', '9');
    std::cout << alphabet.intervals << std::endl;

    std::vector<char> test = {'f', 'H', 'o', '3', ' ', '!'};
    std::vector<int> res = {'a', 'A', 'a', '0', -1, -1};
    for (auto i = 0; i < test.size(); ++i) {
        int cat = alphabet.categorize(test[i]);
        if (cat != res[i]) {
            return false;
        }
    }
    return true;
}

int main()
{
    bool passed = test_categorize();
    std::cout << passed << std::endl;

    Expr expr;
    auto lower = symbol('a', 'z');
    auto upper = symbol('A', 'Z');
    auto zero = symbol('0');

    std::cout << lower->alphabet << std::endl;
    std::cout << upper->alphabet << std::endl;
    std::cout << zero->alphabet << std::endl;

    auto re = (lower | upper) + zero;
    std::cout << re->alphabet << std::endl;

    auto X = symbol('0', '6');
    auto Y = symbol('4', '9');
    std::cout << (X | Y)->alphabet << std::endl;
    std::cout << (X | Y) << std::endl;
}
