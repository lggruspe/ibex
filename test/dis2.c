#include "dis2.h"
#include "test_lib.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void print_interval(const void *pin)
{
    struct dis2_interval in = *((struct dis2_interval*)pin);
    printf("[%d, %d] ", in.start, in.end);
}

size_t node_weight(struct rb2_tree *node)
{
    if (!node) return 0;
    return 1 + node_weight(node->left) + node_weight(node->right);
}

size_t weight(struct dis2_set* set)
{
    return node_weight(set->tree);
}

size_t node_height(struct rb2_tree *node)
{
    if (!node) return 0;
    size_t l = node_height(node->left);
    size_t r = node_height(node->right);
    return 1 + (l > r ? l : r);
}

size_t height(struct dis2_set *set)
{
    return node_height(set->tree);
}

bool test_dis2_duplicate()
{
    bool passed = true;
    struct dis2_set intervals = dis2_create();
    dis2_insert(&intervals, 0, 1);
    dis2_insert(&intervals, 0, 1);

    struct dis2_interval in = *((struct dis2_interval*)(intervals.tree->data));

    check_assertion(weight(&intervals) == 1);
    check_assertion(in.start == 0);
    check_assertion(in.end == 1);
    dis2_destroy(&intervals);
    return passed;
}

int test_dis2_height()
{
    bool passed = true;
    struct dis2_set intervals = dis2_create();

    size_t n = 100;
    int start[n];
    int end[n];
    for (size_t i = 0; i < n; ++i) {
        end[i] = rand() % 100;
        start[i] = rand() % (end[i] + 1);
        assert(start[i] <= end[i]);
        printf("iteration %zu: start=%d end=%d\n", i, start[i], end[i]);
        dis2_insert(&intervals, start[i], end[i]);
    }

    check_assertion(intervals.tree);
    rb2_inorder(intervals.tree, print_interval);

    check_assertion((double)(height(&intervals)) < 6*log2(n + 1.0));
    dis2_destroy(&intervals);
    return passed;
}

int main()
{
    run_test(test_dis2_height);
    //run_test(test_dis2_duplicate);
    return exit_test();
}
