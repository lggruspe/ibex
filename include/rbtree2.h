#pragma once
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

enum rb2_color { rb2_black=0, rb2_red=1 } rb2_color;

struct rb2_tree {
    void *data;
    struct rb2_tree *left, *right, *parent;
    enum rb2_color color;
};

struct rb2_tree *rb2_tree_create(
        const void *data,
        struct rb2_tree *left, 
        struct rb2_tree *right, 
        struct rb2_tree *parent, 
        enum rb2_color color,
        size_t nbytes)  // sizeof(data)
{
    struct rb2_tree *tree = (struct rb2_tree*)malloc(sizeof(struct rb2_tree));
    if (tree) {
        tree->data = malloc(nbytes);
        if (tree->data) {
            tree->left = left;
            tree->right = right;
            tree->parent = parent;
            tree->color = color;
            memcpy(tree->data, data, nbytes);
        } else {
            free(tree);
            tree = NULL;
        }
    }
    return tree;
}

// returns (possibly) new root of tree
struct rb2_tree *rb2_rotate_left(struct rb2_tree *root, struct rb2_tree *x)
{
    assert(x && x->right);
    struct rb2_tree *y = x->right;
    y->parent = x->parent;
    if (y->parent) {
        if (y->parent->left == x) {
            y->parent->left = y;
        } else {
            y->parent->right = y;
        }
    } else {
        root = y;
    }

    x->right = y->left;
    if (x->right) {
        x->right->parent = x;
    }

    y->left = x;
    x->parent = y;
    return root;
}

struct rb2_tree *rb2_rotate_right(struct rb2_tree *root, struct rb2_tree *y)
{
    assert(y && y->left);
    struct rb2_tree *x = y->left;
    x->parent = y->parent;
    if (x->parent) {
        if (x->parent->left == y) {
            x->parent->left = x;
        } else {
            x->parent->right = x;
        }
    } else {
        root = x;
    }

    y->left = x->right;
    if (y->left) {
        y->left->parent = y;
    }

    x->right = y;
    y->parent = x;
    return root;
}

void rb2_destroy(struct rb2_tree *node, bool cascade_, bool free_)
{
    if (!node) return;
    if (cascade_) {
        rb2_destroy(node->left, cascade_, free_);
        rb2_destroy(node->right, cascade_, free_);
    }
    free(node->data);
    if (free_) free(node);
}

struct rb2_tree *rb2_closest_match(
        struct rb2_tree *root,
        const void *data,
        int (*compare)(const void *, const void *))
{
    struct rb2_tree *parent = NULL;
    struct rb2_tree *child = root;
    int comparison;
    while (child && (comparison = compare(data, child->data)) != 0) {
        parent = child;
        child = comparison < 0 ? child->left : child->right;
    }
    return child ? child : parent;
}

struct rb2_tree *rb2_search(
        struct rb2_tree *root, 
        const void *data,
        int (*compare)(const void *, const void *))
{
    struct rb2_tree *node = root;
    int comparison;
    while (node && (comparison = compare(data, node->data)) != 0) {
        node = comparison < 0 ? node->left : node->right;
    }
    return node;
}

struct rb2_tree *rb2_transplant(
        struct rb2_tree *root, 
        struct rb2_tree *u, 
        struct rb2_tree *v)
{
    if (!u->parent) {
        root = v;
    } else if (u->parent->left == u) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    if (v) {
        v->parent = u->parent;
    }
    return root;
}

struct rb2_tree *rb2_repair(struct rb2_tree *root, struct rb2_tree *node)
{
    if (!root || !node) return root;
    struct rb2_tree *parent = node->parent;
    struct rb2_tree *grandparent = (parent ? parent->parent : NULL);
    struct rb2_tree *uncle = NULL;
    if (grandparent) {
        uncle = (grandparent->left == parent 
            ? grandparent->right 
            : grandparent->left);
    }

    // case 1: red uncle
    if (uncle && uncle->color == rb2_red) {
        if (grandparent) {
            grandparent->color = rb2_red;
            uncle->color = rb2_black;
            parent->color = rb2_black;
            return rb2_repair(root, grandparent);
        }
    }
    // case 2: black uncle, triangle
    if (grandparent && grandparent->left == parent && parent->right == node) {
        root = rb2_rotate_left(root, parent);
    } else if (grandparent && grandparent->right == parent && parent->left == node) {
        root = rb2_rotate_right(root, parent);
    }

    // case 3: black uncle, line
    if (grandparent && grandparent->left == parent && parent->left == node) {
        grandparent->color = rb2_red;
        parent->color = rb2_black;
        root = rb2_rotate_right(root, grandparent);
    } else if (grandparent && grandparent->right == parent && parent->right == node) {
        grandparent->color = rb2_red;
        parent->color = rb2_black;
        root = rb2_rotate_left(root, grandparent);
    }
    root->color = rb2_black;
    return root;
}

struct rb2_insert_result {
    struct rb2_tree *root;
    struct rb2_tree *replaced;
};

// returns node that the child replaces, if any, 
// so the caller can deallocate memory (using rb2_destroy(node, false))
struct rb2_insert_result rb2_insert(
        struct rb2_tree *root, 
        struct rb2_tree *node,
        int (*compare)(const void *, const void *))
{
    struct rb2_insert_result result = {
        .root = root,
        .replaced = NULL
    };
    if (!node) return result;

    node->color = rb2_red;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    if (!root) {
        node->color = rb2_black;
        result.root = node;
        return result;
    }

    struct rb2_tree *closest = rb2_closest_match(root, node->data, compare);
    int comparison = compare(node->data, closest->data);
    if (comparison == 0) {
        result.replaced = closest;
        node->color = closest->color;
        node->left = closest->left;
        node->right = closest->right;
        root = rb2_transplant(root, closest, node);
    } else if (comparison < 0) {
        closest->left = node;
        node->parent = closest;
    } else {
        closest->right = node;
        node->parent = closest;
    }
    root = rb2_repair(root, node);
    result.root = root;
    return result;
}

// NOTE doesnt check if new tree preserves BST property
void rb2_node_change_data(struct rb2_tree *node, const void *data, size_t nbytes)
{
    if (node) {
        assert(node->data);
        if (node->data) {
            memcpy(node->data, data, nbytes);
        }
    }
}

void rb2_inorder(const struct rb2_tree *node, void (*handler)(const void*))
{
    if (node) {
        rb2_inorder(node->left, handler);
        handler(node->data);
        rb2_inorder(node->right, handler);
    }
}

void rb2_preorder(const struct rb2_tree *node, void (*handler)(const void*))
{
    if (node) {
        handler(node->data);
        rb2_preorder(node->left, handler);
        rb2_preorder(node->right, handler);
    }
}

void rb2_postorder(const struct rb2_tree *node, void (*handler)(const void*))
{
    if (node) {
        rb2_postorder(node->left, handler);
        rb2_postorder(node->right, handler);
        handler(node->data);
    }
}
