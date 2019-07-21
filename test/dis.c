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
    for (size_t i = 0; i < n; ++i) {
        int end = rand() % 100;
        int start = rand() % end;
        printf("iteration %zu: start=%d end=%d\n", i, start, end);
        dis_insert(&intervals, start, end);
    }

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
