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

/*
#include "dis.h"
#include "test_lib.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void print_interval(void *pin)
{
    struct dis_interval in = *((struct dis_interval*)pin);
    printf("[%d, %d] ", in.start, in.end);
}

size_t node_weight(struct rb_node *node)
{
    if (!node) return 0;
    return 1 + node_weight(node->left) + node_weight(node->right);
}

size_t weight(struct dis_set* set)
{
    return node_weight(set->root);
}

size_t node_height(struct rb_node *node)
{
    if (!node) return 0;
    size_t l = node_height(node->left);
    size_t r = node_height(node->right);
    return 1 + (l > r ? l : r);
}

size_t height(struct dis_set *set)
{
    return node_height(set->root);
}

bool test_dis_duplicate()
{
    bool passed = true;
    struct dis_set intervals = dis_create();
    dis_insert(&intervals, 0, 1);
    dis_insert(&intervals, 0, 1);

    struct dis_interval in = *((struct dis_interval*)(intervals.root->data));

    check_assertion(weight(&intervals) == 1);
    check_assertion(in.start == 0);
    check_assertion(in.end == 1);
    dis_destroy(&intervals);
    return passed;
}

int test_dis_height()
{
    bool passed = true;
    struct dis_set intervals = dis_create();

    size_t n = 100;
    int start[n];
    int end[n];
    for (size_t i = 0; i < n; ++i) {
        end[i] = rand() % 100;
        start[i] = rand() % (end[i] + 1);
        printf("iteration %zu: start=%d end=%d\n", i, start[i], end[i]);
        dis_insert(&intervals, start[i], end[i]);
    }

    check_assertion(intervals.root);
    rb_node_inorder(intervals.root, print_interval);

    check_assertion((double)(height(&intervals)) < 6*log2(n + 1.0));
    dis_destroy(&intervals);
    return passed;
}

int main()
{
    run_test(test_dis_height);
    run_test(test_dis_duplicate);
    return exit_test();
}
*/
