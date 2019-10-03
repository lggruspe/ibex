#pragma once
#include "distree/distree.h"

namespace rnd
{

struct SymbolRange {
    int start, end;
};

struct Alphabet {
    distree::DisSet set;

    void insert(const SymbolRange& interval)
    {
        set.insert(distree::Interval(interval.start, interval.end));
    }

    void combine(const Alphabet& other)
    {
        set.combine(other.set);
    }

    distree::DisSet::Tree begin() const
    {
        return set.tree;
    }

    // returns leftmost interval that overlaps with interval
    distree::DisSet::Tree first_overlap(int start, int end) const
    {
        return set.first_overlap(start, end);
    }
};

template <class T, class U>
bool overlaps(const T& t, const U& u)
{
    distree::Interval it(t.start, t.end);
    distree::Interval jt(u.start, u.end);
    return it == jt;
}

SymbolRange symbol_range(int start, int end);

bool is_empty(const SymbolRange&);

std::ostream& operator<<(std::ostream&, const SymbolRange&);

bool operator<(const SymbolRange&, const SymbolRange&);

}
