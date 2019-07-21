#pragma once
#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef enum rb_color { rb_black=0, rb_red=1 } rb_color;

struct rb_node {
    void *data;
    struct rb_node *left, *right, *parent;
    enum rb_color color;
};

struct rb_node *rb_node_create(
        void *data,
        struct rb_node *left, 
        struct rb_node *right, 
        struct rb_node *parent, 
        rb_color color,
        size_t nbytes)  // number of bytes in data
{
    struct rb_node *node = malloc(sizeof(struct rb_node));
    if (node) {
        node->data = malloc(nbytes);
        if (!node->data) {
            free(node->data);
            free(node);
            node = NULL;
        } else {
            memcpy(node->data, data, nbytes);
            node->left = left;
            node->right = right;
            node->parent = parent;
            node->color = color;
        }
    }
    return node;
}

struct rb_node *rb_node_minimum(struct rb_node *root)
{
    if (!root) {
        return NULL;
    }
    struct rb_node *node = root;
    while (node->left) {
        node = node->left;
    }
    return node;
}

struct rb_node *rb_node_successor(struct rb_node *node)
{
    if (!node) {
        return NULL;
    }
    if (node->right) {
        return rb_node_minimum(node->right);
    }
    struct rb_node *parent = node->parent;
    struct rb_node *child = node;
    while (parent && parent->left != child) {
        child = parent;
        parent = parent->parent;
    }
    return parent;
}

struct rb_tree {
    struct rb_node *root;
    size_t nbytes;      // number of bytes in an element
    int (*comparator)(const void*, const void*);
};

struct rb_tree rb_tree_create(
        size_t nbytes,
        int (*compare)(const void*, const void*))
{
    return (struct rb_tree) {
        .root = NULL,
        .nbytes = nbytes,
        .comparator = compare
    };
}

void rb_rotate_left(struct rb_tree *tree, struct rb_node *x)
{
    assert(x && x->right);
    struct rb_node *y = x->right;
    y->parent = x->parent;
    if (y->parent) {
        if (y->parent->left == x) {
            y->parent->left = y;
        } else {
            y->parent->right = y;
        }
    } else {
        tree->root = y;
    }

    x->right = y->left;
    if (x->right) {
        x->right->parent = x;
    }

    y->left = x;
    x->parent = y;
}

void rb_rotate_right(struct rb_tree *tree, struct rb_node *y)
{
    assert(y && y->left);
    struct rb_node *x = y->left;
    x->parent = y->parent;
    if (x->parent) {
        if (x->parent->left == y) {
            x->parent->left = x;
        } else {
            x->parent->right = x;
        }
    } else {
        tree->root = x;
    }

    y->left = x->right;
    if (y->left) {
        y->left->parent = y;
    }

    x->right = y;
    y->parent = x;
}

void rb_node_destroy(struct rb_node *node, int cascade)
{
    if (!node) {
        return;
    }
    if (cascade) {
        rb_node_destroy(node->left, cascade);
        rb_node_destroy(node->right, cascade);
    }
    free(node->data);
    free(node);
}

void rb_tree_destroy(struct rb_tree *tree, int cascade)
{
    rb_node_destroy(tree->root, cascade);
}

struct rb_node *rb_tree_search(struct rb_tree *tree, void *data)
{
    struct rb_node *node = tree->root;
    while (node) {
        int comparison = (tree)->comparator(data, node->data);
        if (comparison == 0) break;
        node = comparison < 0 ? node->left : node->right;
    }
    return node;
}

void rb_tree_repair(struct rb_tree *tree, struct rb_node *node)
{
    if (!node) {
        return;
    }
    struct rb_node *parent = node->parent;
    struct rb_node *grandparent = (parent ? parent->parent : NULL);
    struct rb_node *uncle = NULL;
    if (grandparent) {
        uncle = (grandparent->left == parent 
            ? grandparent->right 
            : grandparent->left);
    }

    // case 1: red uncle
    if (uncle && uncle->color == rb_red) {
        if (grandparent) {
            grandparent->color = rb_red;
            uncle->color = rb_black;
            parent->color = rb_black;
            return rb_tree_repair(tree, grandparent);
        }
    }
    // case 2: black uncle, triangle
    if (grandparent && grandparent->left == parent && parent->right == node) {
        rb_rotate_left(tree, parent);
    } else if (grandparent && grandparent->right == parent && parent->left == node) {
        rb_rotate_right(tree, parent);
    }

    // case 3: black uncle, line
    if (grandparent && grandparent->left == parent && parent->left == node) {
        grandparent->color = rb_red;
        parent->color = rb_black;
        rb_rotate_right(tree, grandparent);
    } else if (grandparent) {
        grandparent->color = rb_red;
        parent->color = rb_black;
        rb_rotate_left(tree, grandparent);
    }

    tree->root->color = rb_black;
}

void rb_tree_transplant(struct rb_tree *tree, struct rb_node *u, struct rb_node *v)
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

// does not check if inserting child affects tree shape
// or if parent has room for child
// returns node that the child replaces, if any, 
// so the caller can deallocate memory
struct rb_node *rb_tree_insert_child(
        struct rb_tree *tree, 
        struct rb_node *parent, 
        struct rb_node *child)
{
    struct rb_node *replaced = NULL;
    child->color = rb_red;
    child->left = NULL;
    child->right = NULL;
    child->parent = parent;
    if (!parent) {
        tree->root = child;
    } else {
        int comparison = tree->comparator(child->data, parent->data);
        if (comparison == 0) {
            replaced = parent;
            child->color = replaced->color;
            child->left = replaced->left;
            child->right = replaced->right;
            rb_tree_transplant(tree, replaced, child);
        } else if (comparison < 0) {
            parent->left = child;
        } else {
            parent->right = child;
        }
    }
    rb_tree_repair(tree, child);
    return replaced;
}

struct rb_node *rb_find_match_or_parent(
        struct rb_tree *tree,
        struct rb_node *node)
{
    struct rb_node *parent = NULL;
    struct rb_node *child = tree->root;
    int comparison;
    while (child && (comparison = tree->comparator(node->data, child->data)) != 0) {
        parent = child;
        child = comparison < 0 ? child->left : child->right;
    }
    return child ? child : parent;
}

void rb_tree_insert(struct rb_tree *tree, struct rb_node *node)
{
    if (node) {
        struct rb_node *result = rb_find_match_or_parent(tree, node);
        rb_tree_insert_child(tree, result, node);
    }
}

// doesn't preserve black-red properties
void rb_tree_delete(struct rb_tree *tree, struct rb_node *node)
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
    struct rb_node *successor = rb_node_successor(node);
    if (successor->parent != node) {
        rb_tree_transplant(tree, successor, successor->right);
        successor->right = node->right;
        successor->right->parent = successor;
    }
    successor->left = node->left;
    successor->left->parent = successor;
    rb_tree_transplant(tree, node, successor);
}

// NOTE doesnt check if new data preserves red-black properties
void rb_tree_change_node_data(struct rb_tree *tree, struct rb_node *node, const void *data)
{
    if (node) {
        if (!node->data) {
            node->data = malloc(tree->nbytes);
        }
        if (node->data) {
            memcpy(node->data, data, tree->nbytes);
        }
    }
}

void rb_node_inorder(struct rb_node *node, void (*handler)(void*))
{
    if (node) {
        rb_node_inorder(node->left, handler);
        handler(node->data);
        rb_node_inorder(node->right, handler);
    }
}

void rb_node_preorder(struct rb_node *node, void (*handler)(void*))
{
    if (node) {
        handler(node->data);
        rb_node_preorder(node->left, handler);
        rb_node_preorder(node->right, handler);
    }
}

void rb_node_postorder(struct rb_node *node, void (*handler)(void*))
{
    if (node) {
        rb_node_postorder(node->left, handler);
        rb_node_postorder(node->right, handler);
        handler(node->data);
    }
}
