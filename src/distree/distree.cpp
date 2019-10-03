#include "distree.h"
#include "redblack/tree.hpp"
#include <algorithm>
#include <iostream>

namespace distree
{

Interval::operator bool() const
{
    return start != 0 || end != 0;
}

bool Interval::operator<(const Interval& other) const
{
    return end < other.start;
}

bool Interval::operator!=(const Interval& other) const
{
    return *this < other || other < *this;
}

bool Interval::operator==(const Interval& other) const
{
    return !(*this < other) && !(other < *this);
}

bool DisSet::contains(int point) const
{
    auto node = rb::search(this->tree, Interval(point, point));
    return node && node->data.start <= point && point <= node->data.end;
}

DisSet::Tree insert(DisSet::Tree root, DisSet::Tree node, Interval interval)
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

void DisSet::clear()
{
    rb::destroy(tree);
    tree = nullptr;
}

void DisSet::insert(const Interval& a)
{
    tree = distree::insert(tree, tree, a);
}

void DisSet::combine(const DisSet& other)
{
    auto node = rb::minimum(other.tree);
    while (node) {
        insert(node->data);
        node = rb::successor(node);
    }
}

std::pair<DisSet::Tree, DisSet::Tree> DisSet::overlap_range(const Interval& a) const
{
    return rb::equal_range(this->tree, a);
}

std::ostream& operator<<(std::ostream& out, const Interval& interval)
{
    if (interval.start == interval.end) {
        return out << interval.start;
    }
    return out << "[" << interval.start << ", " << interval.end << "]";
}

std::ostream& operator<<(std::ostream& out, DisSet::Tree node)
{
    if (node) {
        out << node->left;
        out << node->data << " ";
        out << node->right;
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, const DisSet& set)
{
    return out << "{" << set.tree << "}";
}

} // end namespace
