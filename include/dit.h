#pragma once
#include "rb/node.h"
#include "rb/tree.h"

typedef struct {
    int start, end;
} interval_t;

typedef struct {
    rb_tree_t *tree;
} disjoint_interval_set_t;

int interval_compare(const void *a, const void *b)
{
    // -1: a.end < b.start
    // 0: a and b overlap
    // 1: a.start > b.end
    interval_t in = *((interval_t*)a);
    interval_t jn = *((interval_t*)b);
    if (in.end < jn.start) {
        return -1;
    } else if (in.start > jn.end) {
        return 1;
    } else {
        return 0;
    }
}

disjoint_interval_set_t *disjoint_interval_set_create()
{
    disjoint_interval_set_t *intervals = malloc(sizeof(disjoint_interval_set_t));
    if (intervals) {
        intervals->tree = rb_tree_create(sizeof(interval_t), interval_compare);
        if (!intervals->tree) {
            free(intervals);
            intervals = NULL;
        }
    }
    return intervals;
}

void disjoint_interval_set_destroy(
        disjoint_interval_set_t *intervals, 
        int cascade)
{
    rb_tree_destroy(intervals->tree, cascade);
    free(intervals);
}

interval_t interval_create(int start, int end)
{
    interval_t in;
    in.start = start;
    in.end = end;
    return in;
}

int int_compare(const void *a, const void *b)
{
    int x = *((int*)a);
    int y = *((int*)b);
    if (x < y) {
        return -1;
    } else if (x == y) {
        return 0;
    } else {
        return 1;
    }
}

void set_boundary(int boundary[], interval_t in, interval_t jn)
{
    boundary[0] = in.start;
    boundary[1] = in.end;
    boundary[2] = jn.start;
    boundary[3] = jn.end;
    qsort(boundary, 4, sizeof(int), int_compare);
}

void disjoint_interval_set_insert(
        disjoint_interval_set_t *intervals,
        interval_t in)
{
    if (in.start > in.end) {
        return;
    }
    if (!intervals->tree->root) {
        intervals->tree->root = rb_node_create(&in, NULL, NULL, NULL, RB_BLACK, intervals->tree->nbytes);
        return;
    }

    // find an interval that overlaps
    rb_node_t *node = rb_node_find_match_or_parent(intervals->tree->root, &in, interval_compare);
    int comparison = interval_compare(&in, node->key);
    if (comparison != 0) {
        rb_node_t *new_node = rb_node_create(&in, NULL, NULL, node, RB_RED, 
                intervals->tree->nbytes);
        rb_tree_insert_child(intervals->tree, node, new_node);
    } else {
        int boundary[4];
        set_boundary(boundary, *((interval_t*)(node->key)), in);
        interval_t intersection = interval_create(boundary[1], boundary[2]);
        rb_tree_change_node_key(intervals->tree, node, &intersection);
        disjoint_interval_set_insert(intervals, 
                interval_create(boundary[0], boundary[1] - 1));
        disjoint_interval_set_insert(intervals,
                interval_create(boundary[2] + 1, boundary[3]));
    }
}
