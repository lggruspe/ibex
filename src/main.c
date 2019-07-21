#include "dit.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void print_interval(struct rb_node *node)
{
    struct dis_interval in = *((struct dis_interval*)(node->data));
    printf("[%d, %d] ", in.start, in.end);
}

int count(struct rb_node *node)
{
    if (!node) {
        return 0;
    }
    return 1 + count(node->left) + count(node->right);
}

int height(struct rb_node *node)
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
    struct dis_set intervals = dis_create();
    dis_insert(&intervals, 0, 1);
    dis_insert(&intervals, 0, 1);

    struct dis_interval in = *((struct dis_interval*)(intervals.root->data));
    int passed = count(intervals.root) == 1
        && in.start == 0
        && in.end == 1;
    dis_destroy(&intervals);
    return passed;
}

int test_disjoint_interval_set_height(int n)
{
    struct dis_set intervals = dis_create();

    for (int i = 0; i < n; ++i) {
        int end = rand() % 100;
        int start = rand() % end;
        dis_insert(&intervals, start, end);
    }

    int passed = (double)(height(intervals.root)) < 2*log2(n + 1.0);
    dis_destroy(&intervals);
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
