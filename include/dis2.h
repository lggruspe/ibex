#pragma once
#include "rbtree2.h"

struct dis2_interval {
    int start, end;
};

struct dis2_set {
    struct rb2_tree *tree;
};

int dis2_interval_compare(const void *a, const void *b)
{
    // returns 0 if a and b overlap
    struct dis2_interval in = *((struct dis2_interval*)a);
    struct dis2_interval jn = *((struct dis2_interval*)b);
    if (in.end < jn.start) {
        return -1;
    } else if (in.start > jn.end) {
        return 1;
    } else {
        return 0;
    }
}

struct dis2_set dis2_create()
{
    return (struct dis2_set) {
        .tree = NULL
    };
}

void dis2_destroy(struct dis2_set *set)
{
    rb2_destroy(set->tree, true, true);
}

int dis2_int_compare(const void *a, const void *b)
{
    return *((int*)a) - *((int*)b);
}

void set_boundary(int boundary[], struct dis2_interval in, struct dis2_interval jn)
{
    boundary[0] = in.start;
    boundary[1] = in.end;
    boundary[2] = jn.start;
    boundary[3] = jn.end;
    qsort(boundary, 4, sizeof(int), dis2_int_compare);
}

void dis2_insert(struct dis2_set *set, int start, int end)
{
    if (start > end) return;
    struct dis2_interval in = {
        .start = start,
        .end = end
    };
    if (!set->tree) {
        struct rb2_tree *node = rb2_tree_create(&in, NULL, NULL, NULL, rb2_black, sizeof(struct dis2_interval));
        struct rb2_insert_result result = rb2_insert(set->tree, node, dis2_interval_compare);
        set->tree = result.root;
        return;
    }

    // find an interval that overlaps
    // TODO use rb2_find_match_or_parent
    struct rb2_tree *node = rb2_search(set->tree, &in, dis2_interval_compare);
    if (!node) {
        struct rb2_tree *new_node = rb2_tree_create(&in, NULL, NULL, node, 
                rb2_red, sizeof(struct dis2_interval));
        rb2_insert(set->tree, new_node, dis2_interval_compare);
    } else {
        int boundary[4];
        set_boundary(boundary, *((struct dis2_interval*)(node->data)), in);
        struct dis2_interval intersection = {
            .start = boundary[1],
            .end = boundary[2]
        };
        rb2_node_change_data(node, &intersection, sizeof(struct dis2_interval));
        // TODO you shouldn't have to start at the root
        dis2_insert(set, boundary[0], boundary[1] - 1);
        dis2_insert(set, boundary[2] + 1, boundary[3]);
    }
}
