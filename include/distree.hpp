#pragma once
#include "tree.hpp"
#include <stdexcept>

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

    explicit operator bool() const;
    bool operator<(const Interval& other) const;
    bool operator!=(const Interval& other) const;
    bool operator==(const Interval& other) const;
};

using DisTree = rb::Tree<Interval>;

bool contains(DisTree* tree, int point);

DisTree* insert(DisTree* root, DisTree* node, Interval interval);

struct DisSet {
    DisTree* tree;

    DisSet() : tree(nullptr) {}
    ~DisSet()
    {
        clear();
    }

    void clear();
    void insert(int a, int b);
    void combine(const DisSet& other);
    // leftmost interval that overlaps
    DisTree* first_overlap(Interval interval) const;
};

} // end namespace
