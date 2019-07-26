#include "distree.hpp"
#include <cstdlib>
#include <iostream>

using namespace distree;

std::ostream& operator<<(std::ostream& out, DisTree* tree)
{
    if (tree) {
        out << tree->left;
        out << "[" << tree->data.start << ", " << tree->data.end << "] ";
        out << tree->right;
    }
    return out;
}


std::ostream& operator<<(std::ostream& out, DisSet& set)
{
    return out << set.tree << std::endl;
}

void print_set()
{
    size_t n = 30;
    int start[n];
    int end[n];

    DisSet set;
    for (size_t i = 0; i < n; ++i) {
        std::cout << "i = " << i << std::endl;
        std::cout << "set = " << set << std::endl;
        end[i] = rand() % 100;
        start[i] = rand() % (end[i] + 1);
        set.insert(start[i], end[i]);

        std::cout << "(start, end)(i) = (" << start[i] << ", " << end[i] << ")" << std::endl;
    }
    std::cout << "i = " << n << ", set = " << set << std::endl;
}

int main()
{
    //print_set();
    DisSet A, B;

    A.insert(1, 3);
    B.insert(2, 4);
    A.combine(B);
    std::cout << "A = " << A << std::endl;
    std::cout << "B = " << B << std::endl;
}
