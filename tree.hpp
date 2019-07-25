#pragma once
#include <stdexcept>
#include <utility>

namespace rb
{

enum class Color { black, red };

template <class T>
struct Tree {
    T data;
    Tree<T> *left, *right, *parent;
    Color color;

    Tree(
            const T& data, 
            Color color,
            Tree<T> *parent=nullptr,
            Tree<T> *left=nullptr,
            Tree<T> *right=nullptr)
        : data(data)
        , color(color)
        , parent(parent)
        , left(left)
        , right(right)
    {}

    bool operator<(const Tree<T>& other)
    {
        return data < other.data;
    }
};

template <class T>
Tree<T>* search(Tree<T>* tree, const T& data)
{
    auto node = tree;
    while (node && node->data != data) {
        node = data < node->data ? node->left : node->right;
    }
    return node;
}

template <class T>
Tree<T>* rotate_left(Tree<T>* root, Tree<T>* x)
{
    if (!root || !x || !x->right) {
        throw std::invalid_argument("inputs must not be null");
    }

    auto y = x->right;
    x->right = y->left;
    if (x->right) {
        x->right->parent = x;
    }

    y->left = x;
    y->parent = x->parent;
    x->parent = y;
    if (!y->parent) {
        root = y;
    } else if (y->parent->left == x) {
        y->parent->left = y;
    } else if (y->parent->right == x) {
        y->parent->right = y;
    }
    return root;
}

template <class T>
Tree<T>* rotate_right(Tree<T>* root, Tree<T>* y)
{
    if (!root || !y || !y->left) {
        throw std::invalid_argument("inputs must not be null");
    }

    auto x = y->left;
    y->left = x->right;
    if (y->left) {
        y->left->parent = y;
    }

    x->right = y;
    x->parent = y->parent;
    y->parent = x;
    if (!x->parent) {
        root = x;
    } else if (x->parent->right == y) {
        x->parent->right = x;
    } else if (x->parent->left == y) {
        x->parent->left = x;
    }
    return root;

}

template <class T>
Tree<T>* closest_match(Tree<T>* tree, const T& data)
{
    Tree<T>* parent = nullptr;
    auto child = tree;
    while (child && child->data != data) {
        parent = child;
        child = data < child->data ? child->left : child->right;
    }
    return child ? child : parent;
}

template <class T>
Tree<T>* repair(Tree<T>* root, Tree<T>* node)
{
    if (!root) {
        return nullptr;
    } else if (!node) {
        return root;
    }

    auto parent = node->parent;
    auto grandparent = parent ? parent->parent : nullptr;
    auto uncle = !grandparent ? nullptr : (grandparent->left == parent 
            ? grandparent->right 
            : grandparent->left);

    if (parent && parent->color == Color::red) {
        if (uncle && uncle->color == Color::red) {
            grandparent->color = Color::red;
            parent->color = Color::black;
            uncle->color = Color::black;
            return repair(root, grandparent);
        } else if (grandparent) {
            if (grandparent->left == parent && parent->right == node) {
                root = rotate_left(root, parent);
            } else if (grandparent->right == parent && parent->left == node) {
                root = rotate_right(root, parent);
            }

            if (grandparent->left == parent && parent->left == node) {
                grandparent->color = Color::red;
                parent->color = Color::black;
                root = rotate_right(root, grandparent);
            } else if (grandparent->right == parent && parent->right == node) {
                grandparent->color = Color::red;
                parent->color = Color::black;
                root = rotate_left(root, grandparent);
            }
        }
    }
    root->color = Color::black;
    return root;
}

template <class T>
std::pair<Tree<T>*, Tree<T>*> insert(
        Tree<T>* root,
        Tree<T>* node,
        Tree<T>* location=nullptr)
{
    if (!node) {
        return { root, nullptr };
    } else if (!root) {
        node->color = Color::black;
        return { node, nullptr };
    }
    if (!location) {
        location = root;
    }
    auto closest = closest_match(location, node->data);
    if (node == closest) {
        node->left = closest->left;
        node->right = closest->right;
        node->parent = closest->parent;
        node->color = closest->color;
        if (node->left) {
            node->left->parent = node;
        }
        if (node->right) {
            node->right->parent = node;
        }
        if (!node->parent) {
            root = node;
        } else if (node->parent->left == closest) {
            node->parent->left = node;
        } else if (node->parent->right == closest) {
            node->parent->right = node;
        }
    } else {
        (node < closest ? closest->left : closest->right) = node;
        node->parent = closest;
        node->color = Color::red;
    }
    return { repair(root, node), node == closest ? closest : nullptr };
}

} // end namespace
