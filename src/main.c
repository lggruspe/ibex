#include "dit.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void print_interval(rb_node_t *node)
{
    interval_t in = *((interval_t*)(node->key));
    printf("[%d, %d] ", in.start, in.end);
}

int count(rb_node_t *node)
{
    if (!node) {
        return 0;
    }
    return 1 + count(node->left) + count(node->right);
}

int height(rb_node_t *node)
{
    if (!node) {
        return 0;
    }
    int l = height(node->left);
    int r = height(node->right);
    return 1 + (l > r ? l : r);
}

int test_disjoint_interval_set_duplicate()
{
    disjoint_interval_set_t *intervals = disjoint_interval_set_create();
    if (!intervals) {
        return 0;
    }

    disjoint_interval_set_insert(intervals, interval_create(0, 1));
    disjoint_interval_set_insert(intervals, interval_create(0, 1));

    interval_t in = *((interval_t*)(intervals->tree->root->key));
    int passed = count(intervals->tree->root) == 1
        && in.start == 0
        && in.end == 1;
    disjoint_interval_set_destroy(intervals, 1);
    return passed;
}

int test_disjoint_interval_set_height(int n)
{
    disjoint_interval_set_t *intervals = disjoint_interval_set_create();
    if (!intervals) {
        return 0;
    }

    for (int i = 0; i < n; ++i) {
        int end = rand() % 100;
        int start = rand() % end;
        interval_t in = interval_create(start, end);
        disjoint_interval_set_insert(intervals, in);
    }

    int passed = (double)(height(intervals->tree->root)) < 2*log2(n + 1.0);
    disjoint_interval_set_destroy(intervals, 1);
    return passed;
}

int main()
{
    // TODO stack overflow when input = 20?
    int passed = test_disjoint_interval_set_height(10);
    printf("height: %d\n", passed);
   
    passed = test_disjoint_interval_set_duplicate();
    printf("duplicate: %d\n", passed);
}
