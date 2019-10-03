#pragma once
#include "redblack/tree.hpp"
#include <iostream>
#include <stdexcept>
#include <utility>

namespace distree
{

struct Interval {
    int start, end;

    Interval(int start, int end) : start(start), end(end) 
    {
        if (start > end) {
            throw std::invalid_argument("interval shouldn't be empty");
        }
    }

    Interval() : Interval(0, 0) {}
    explicit operator bool() const;
    bool operator<(const Interval& other) const;
    bool operator!=(const Interval& other) const;
    bool operator==(const Interval& other) const;
};

struct DisSet {
    using Tree = rb::Tree<Interval>*;
    Tree tree;

    DisSet() : tree(nullptr) {}
    ~DisSet()
    {
        clear();
    }

    void clear();
    void insert(const Interval& a);
    void combine(const DisSet& other);

    std::pair<Tree, Tree> overlap_range(const Interval& a) const;

    bool contains(int point) const;
};

std::ostream& operator<<(std::ostream& out, const Interval& interval);

std::ostream& operator<<(std::ostream& out, rb::Tree<Interval>* node);

std::ostream& operator<<(std::ostream& out, const DisSet& set);

} // end namespace
