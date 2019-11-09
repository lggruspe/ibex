#pragma once
#include <cassert>
#include <iterator>
#include <limits>
#include <set>

struct ZRange {
    unsigned int start, end;

    ZRange(unsigned int start, unsigned int end) : start(start), end(end) {}

    ZRange()
        : ZRange(
            std::numeric_limits<unsigned int>::max(),
            std::numeric_limits<unsigned int>::max())
    {}

    explicit operator bool() const
    {
        return start < end;
    }

    bool operator<(const ZRange& other) const
    {
        // warning: doesn't induce equivalence relation
        return end <= other.start;
    }
};

struct ZPartition {
    std::set<unsigned int> points;

    ZPartition() : points({0}) {}

    void insert(const ZRange& ran)
    {
        if (ran) {
            points.insert(ran.start);
            if (ran.end < std::numeric_limits<unsigned int>::max()) {
                points.insert(ran.end);
            }
        }
    }

    ZPartition combined(const ZPartition& other) const
    {
        ZPartition rv = *this;
        rv.points.insert(other.points.begin(), other.points.end());
        return rv;
    }

    class Iterator {
        std::set<unsigned int>::const_iterator it;
        std::set<unsigned int>::const_iterator jt;
        std::set<unsigned int>::const_iterator end;

    public:
        Iterator(
            const std::set<unsigned int>& points,
            std::set<unsigned int>::const_iterator it)
            : it(it)
            , jt(it == points.end() ? it : std::next(it))
            , end(points.end())
        {}
    
        bool operator==(Iterator& other) const
        {
            return it == other.it && jt == other.jt;
        }

        bool operator!=(Iterator& other) const
        {
            return it != other.it || jt != other.jt;
        }

        ZRange operator*() const
        {
            unsigned int a = std::numeric_limits<unsigned int>::max();
            unsigned int b = a;
            if (it != end) {
                a = *it;
                assert(std::next(it) == jt);
            }
            if (jt != end) {
                b = *jt;
            }
            return ZRange(a, b);
        }

        Iterator& operator++()
        {
            it = jt;
            jt = (jt == end ? end : std::next(jt));
            return *this;
        }

        Iterator operator++(int)
        {
            Iterator rv = *this;
            ++(*this);
            return rv;
        }
    };

    auto begin() const
    {
        return Iterator(points, points.cbegin());
    }

    auto end() const
    {
        return Iterator(points, points.cend());
    }

private:
    Iterator lower_bound(const ZRange& ran) const
    {
        auto lb = points.lower_bound(ran.start);
        return Iterator(points, lb == points.cend() || *lb != ran.start
            ? std::prev(lb)
            : lb);
    }

    Iterator upper_bound(const ZRange& ran) const
    {
        auto ub = points.upper_bound(ran.end);
        assert(ub != points.cbegin()); 
        // because upper_bound(0) = next interval after 0
        auto prev = std::prev(ub);
        return Iterator(points, *prev == ran.end ? prev : ub);
    }

public:
    auto overlap_range(const ZRange& ran) const
    {
        if (!ran) {
            return std::make_pair(
                Iterator(points, points.cend()),
                Iterator(points, points.cend()));
        }
        return std::make_pair(lower_bound(ran), upper_bound(ran));
    }
};
