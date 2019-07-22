#pragma once
#include "rbtree.h"
#define dis_set rb_tree

struct dis_interval {
    int start, end;
};

int dis_interval_compare(const void *a, const void *b)
{
    // returns 0 if a and b overlap
    struct dis_interval in = *((struct dis_interval*)a);
    struct dis_interval jn = *((struct dis_interval*)b);
    if (in.end < jn.start) {
        return -1;
    } else if (in.start > jn.end) {
        return 1;
    } else {
        return 0;
    }
}

struct dis_set dis_create()
{
    return (struct dis_set)rb_tree_create(
            sizeof(struct dis_interval), dis_interval_compare);
}

void dis_destroy(struct dis_set *set)
{
    rb_tree_destroy(set, 1);
}

int int_compare(const void *a, const void *b)
{
    return *((int*)a) - *((int*)b);
}

void set_boundary(int boundary[], struct dis_interval in, struct dis_interval jn)
{
    boundary[0] = in.start;
    boundary[1] = in.end;
    boundary[2] = jn.start;
    boundary[3] = jn.end;
    qsort(boundary, 4, sizeof(int), int_compare);
}

void dis_insert(struct dis_set *intervals, int start, int end)
{
    if (start > end) return;
    struct dis_interval in = {
        .start = start,
        .end = end
    };
    if (!intervals->root) {
        rb_tree_insert(intervals, rb_node_create(&in, NULL, NULL, NULL,
            rb_black, intervals->nbytes));
        return;
    }

    // find an interval that overlaps
    // TODO use rb_find_match_or_parent
    struct rb_node *node = rb_tree_search(intervals, &in);
    if (!node) {
        struct rb_node *new_node = rb_node_create(&in, NULL, NULL, node, 
                rb_red, intervals->nbytes);
        rb_tree_insert(intervals, new_node);
    } else {
        int boundary[4];
        set_boundary(boundary, *((struct dis_interval*)(node->data)), in);
        struct dis_interval intersection = {
            .start = boundary[1],
            .end = boundary[2]
        };
        rb_tree_change_node_data(intervals, node, &intersection);
        // TODO you shouldn't have to start at the root
        dis_insert(intervals, boundary[0], boundary[1] - 1);
        dis_insert(intervals, boundary[2] + 1, boundary[3]);
    }
}
