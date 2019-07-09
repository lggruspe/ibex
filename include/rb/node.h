#pragma once
#include <stdlib.h>
#include <string.h>

const char RB_BLACK = 'b';
const char RB_RED = 'r';

typedef struct rb_node_t {
    void *key;
    struct rb_node_t *left, *right, *parent;
    char color;
} rb_node_t;

rb_node_t *rb_node_create(
        void *key,
        rb_node_t *left, 
        rb_node_t *right, 
        rb_node_t *parent, 
        char color,
        size_t nbytes)  // number of bytes in key
{
    rb_node_t *node = malloc(sizeof(rb_node_t));
    if (node) {
        node->key = malloc(nbytes);
        if (!node->key) {
            free(node->key);
            free(node);
        } else {
            memcpy(node->key, key, nbytes);
            node->left = left;
            node->right = right;
            node->parent = parent;
            node->color = color;
        }
    }
    return node;
}

rb_node_t *rb_node_search(
        rb_node_t *root, 
        void *key, 
        int compare(const void*, const void*))
{
    rb_node_t *node = root;
    while (node) {
        int comparison = compare(key, node->key);
        if (comparison == 0) {
            break;
        }
        node = (comparison < 0 ? node->left : node->right);
    }
    return node;
}

rb_node_t *rb_node_find_match_or_parent(
        rb_node_t *root,
        void *key,
        int compare(const void*, const void*))
{
    rb_node_t *parent = NULL;
    rb_node_t *child = root;
    int comparison;
    while (child && ((comparison = compare(key, child->key)) != 0)) {
        parent = child;
        child = (comparison < 0 ? child->left : child->right);
    }
    return child ? child : parent;
}

rb_node_t *rb_node_minimum(rb_node_t *root)
{
    if (!root) {
        return NULL;
    }
    rb_node_t *node = root;
    while (node->left) {
        node = node->left;
    }
    return node;
}

rb_node_t *rb_node_maximum(rb_node_t *root)
{
    if (!root) {
        return NULL;
    }
    rb_node_t *node = root;
    while (node->right) {
        node = node->right;
    }
    return node;
}

rb_node_t *rb_node_successor(rb_node_t *node)
{
    if (!node) {
        return NULL;
    }
    if (node->right) {
        return rb_node_minimum(node->right);
    }
    rb_node_t *parent = node->parent;
    rb_node_t *child = node;
    while (parent && parent->left != child) {
        child = parent;
        parent = parent->parent;
    }
    return parent;
}

rb_node_t *rb_node_predecessor(rb_node_t *node)
{
    if (!node) {
        return NULL;
    }
    if (node->left) {
        return rb_node_maximum(node->left);
    }
    rb_node_t *parent = node->parent;
    rb_node_t *child = node;
    while (parent && parent->right != child) {
        child = parent;
        parent = parent->parent;
    }
    return parent;
}

// should only be called if x and y are not null
void rb_node_rotate_left(rb_node_t *x)
{
    if (!x) {
        exit(EXIT_FAILURE);
    }
    rb_node_t *y = x->right;
    if (!y) {
        exit(EXIT_FAILURE);
    }
    y->parent = x->parent;
    if (y->parent) {
        if (y->parent->left == x) {
            y->parent->left = y;
        } else {
            y->parent->right = y;
        }
    }

    x->right = y->left;
    if (x->right) {
        x->right->parent = x;
    }

    y->left = x;
    x->parent = y;
}

void rb_node_rotate_right(rb_node_t *y)
{
    if (!y) {
        exit(EXIT_FAILURE);
    }
    rb_node_t *x = y->left;
    if (!x) {
        exit(EXIT_FAILURE);
    }
    x->parent = y->parent;
    if (x->parent) {
        if (x->parent->left == y) {
            x->parent->left = x;
        } else {
            x->parent->right = x;
        }
    }

    y->left = x->right;
    if (y->left) {
        y->left->parent = y;
    }

    x->right = y;
    y->parent = x;
}

void rb_node_destroy(rb_node_t *node, int cascade)
{
    if (!node) {
        return;
    }
    if (cascade) {
        rb_node_destroy(node->left, cascade);
        rb_node_destroy(node->right, cascade);
    }
    free(node->key);
    free(node);
}

void rb_node_inorder(rb_node_t *node, void (*handler)(rb_node_t*))
{
    if (node) {
        rb_node_inorder(node->left, handler);
        handler(node);
        rb_node_inorder(node->right, handler);
    }
}
