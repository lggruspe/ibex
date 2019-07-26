#include "tree.hpp"
#include <algorithm>
#include <stdexcept>
#include <utility>

namespace distree
{
;

struct Interval {
    int start, end;

    Interval(int start, int end) : start(start), end(end) 
    {
        if (start > end) {
            throw std::invalid_argument("interval shouldn't be empty");
        }
    }

    bool operator<(const Interval& other) const
    {
        return end < other.start;
    }

    bool operator!=(const Interval& other) const
    {
        return *this < other || other < *this;
    }

    bool operator==(const Interval& other) const
    {
        return !(*this != other);
    }
};

using DisTree = rb::Tree<Interval>;

bool contains(DisTree* tree, int point)
{
    auto node = rb::search(tree, Interval(point, point));
    return node && node->data.start <= point && point <= node->data.end;
}

DisTree* insert(DisTree* root, DisTree* node, Interval interval)
{
    if (interval.start > interval.end) {
        return root;
    } else if (!root) {
        return new rb::Tree<Interval>(interval, rb::Color::black);
    }

    if (interval < node->data) {
        if (node->left) {
            return insert(root, node->left, interval);
        } else {
            auto new_node = new rb::Tree<Interval>(interval, rb::Color::red, node);
            node->left = new_node;
            return rb::repair(root, new_node);
        }
    } else if (node->data < interval) {
        if (node->right) {
            return insert(root, node->right, interval);
        } else {
            auto new_node = new rb::Tree<Interval>(interval, rb::Color::red, node);
            node->right = new_node;
            return rb::repair(root, new_node);
        }
    }

    int endpoints[] = { node->data.start, node->data.end, interval.start, interval.end };
    std::sort(endpoints, endpoints + 4);
    node->data.start = endpoints[1];
    node->data.end = endpoints[2];
    if (endpoints[0] <= endpoints[1] - 1) {
        root = insert(root, node, Interval(endpoints[0], endpoints[1] - 1));
    }
    if (endpoints[2] + 1 <= endpoints[3]) {
        root = insert(root, node, Interval(endpoints[2] + 1, endpoints[3]));
    }
    return root;
}

struct DisSet {
    DisTree* tree;

    DisSet() : tree(nullptr) {}
    ~DisSet()
    {
        rb::destroy(tree);
    }

    void insert(int a, int b)
    {
        tree = distree::insert(tree, tree, Interval(a, b));
    }
};

} // end namespace
