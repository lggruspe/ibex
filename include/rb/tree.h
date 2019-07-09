#pragma once
#include "node.h"

typedef struct {
    rb_node_t *root;
    size_t nbytes;      // number of bytes in an element
    int (*comparator)(const void*, const void*);
} rb_tree_t;

rb_tree_t *rb_tree_create(
        size_t nbytes,
        int (*compare)(const void*, const void*))
{
    rb_tree_t *tree = malloc(sizeof(rb_tree_t));
    if (tree) {
        tree->root = NULL;
        tree->nbytes = nbytes;
        tree->comparator = compare;
    }
    return tree;
}

void rb_tree_rotate_left(rb_tree_t *tree, rb_node_t *node)
{
    rb_node_rotate_left(node);
    if (tree->root == node) {
        tree->root = node->parent;
    }
}

void rb_tree_rotate_right(rb_tree_t *tree, rb_node_t *node)
{
    rb_node_rotate_right(node);
    if (tree->root == node) {
        tree->root = node->parent;
    }
}

void rb_tree_destroy(rb_tree_t *tree, int cascade)
{
    rb_node_destroy(tree->root, cascade);
    free(tree);
}

rb_node_t *rb_tree_maximum(rb_tree_t *tree)
{
    return rb_node_maximum(tree->root);
}

rb_node_t *rb_tree_minimum(rb_tree_t *tree)
{
    return rb_node_minimum(tree->root);
}

rb_node_t *rb_tree_search(rb_tree_t *tree, void *key)
{
    return rb_node_search(tree->root, key, tree->comparator);
}

void rb_tree_repair(rb_tree_t *tree, rb_node_t *node)
{
    if (!node) {
        return;
    }
    rb_node_t *parent = node->parent;
    rb_node_t *grandparent = (parent ? parent->parent : NULL);
    rb_node_t *uncle = NULL;
    if (grandparent) {
        uncle = (grandparent->left == parent 
            ? grandparent->right 
            : grandparent->left);
    }

    // case 1: red uncle
    if (uncle && uncle->color == RB_RED) {
        if (grandparent) {
            grandparent->color = RB_RED;
            uncle->color = RB_BLACK;
            parent->color = RB_BLACK;
            return rb_tree_repair(tree, grandparent);
        }
    }
    // case 2: black uncle, triangle
    if (grandparent && grandparent->left == parent && parent->right == node) {
        rb_tree_rotate_left(tree, parent);
    } else if (grandparent && grandparent->right == parent && parent->left == node) {
        rb_tree_rotate_right(tree, parent);
    }

    // case 3: black uncle, line
    if (grandparent && grandparent->left == parent && parent->left == node) {
        grandparent->color = RB_RED;
        parent->color = RB_BLACK;
        rb_tree_rotate_right(tree, grandparent);
    } else if (grandparent) {
        grandparent->color = RB_RED;
        parent->color = RB_BLACK;
        rb_tree_rotate_left(tree, grandparent);
    }

    tree->root->color = RB_BLACK;
}

// does not check if inserting child affects tree shape
// or if parent has room for child
void rb_tree_insert_child(rb_tree_t *tree, rb_node_t *parent, rb_node_t *child)
{
    child->color = RB_RED;
    child->left = NULL;
    child->right = NULL;
    child->parent = parent;
    if (!parent) {
        tree->root = child;
    } else {
        int comparison = tree->comparator(child->key, parent->key);
        // what if comparison == 0?
        if (comparison < 0) {
            parent->left = child;
        } else {
            parent->right = child;
        }
    }
    rb_tree_repair(tree, child);
}

void rb_tree_insert(rb_tree_t *tree, rb_node_t *node)
{
    if (!node) {
        return;
    }
    rb_node_t *parent = NULL;
    rb_node_t *child = tree->root;
    while (child) {
        parent = child;
        child = (node->key < child->key ? child->left : child->right);
    }
    rb_tree_insert_child(tree, parent, node);
}

void rb_tree_transplant(rb_tree_t *tree, rb_node_t *u, rb_node_t *v)
{
    if (!u->parent) {
        tree->root = v;
    } else if (u->parent->left == u) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    if (v) {
        v->parent = u->parent;
    }
}

void rb_tree_delete(rb_tree_t *tree, rb_node_t *node)
{
    if (!node) {
        return;
    }
    if (!node->left && !node->right) {
        return rb_tree_transplant(tree, node, NULL);
    } else if (!node->left || !node->right) {
        return rb_tree_transplant(tree, node, 
                node->left ? node->left : node->right);
    }
    rb_node_t *successor = rb_node_successor(node);
    if (successor->parent != node) {
        rb_tree_transplant(tree, successor, successor->right);
        successor->right = node->right;
        successor->right->parent = successor;
    }
    successor->left = node->left;
    successor->left->parent = successor;
    rb_tree_transplant(tree, node, successor);
}

// NOTE doesnt check if new key preserves tree shape
void rb_tree_change_node_key(rb_tree_t *tree, rb_node_t *node, const void *key)
{
    if (node) {
        if (!node->key) {
            node->key = malloc(tree->nbytes);
        }
        if (node->key) {
            memcpy(node->key, key, tree->nbytes);
        }
    }
}
