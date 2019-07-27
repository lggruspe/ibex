#pragma once
#include "distree.h"

// wraps distree::DisSet
struct Alphabet {
    distree::DisSet set;
    using Category = typename distree::Interval;

    void insert(const Category& interval)
    {
        insert(interval.start, interval.end);
    }

    void insert(int a, int b)
    {
        Category interval(a, b);
        set.insert(a, b);
    }

    void combine(const Alphabet& other)
    {
        set.combine(other.set);
    }

    distree::DisTree* begin() const
    {
        return set.tree;
    }

    // returns leftmost interval that overlaps with interval
    distree::DisTree* first_overlap(Category interval) const
    {
        return set.first_overlap(interval);
    }
};


