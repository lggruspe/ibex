#pragma once
#include <cassert>
#include <cstdint>
#include <iterator>
#include <limits>
#include <set>
#include <tuple>

namespace rnd {

// Represents a half-open interval [start, end) with integer endpoints.
struct ZRange {
    uint32_t start = std::numeric_limits<uint32_t>::max();
    uint32_t end = std::numeric_limits<uint32_t>::max();

    ZRange() {}

    ZRange(uint32_t start, uint32_t end) : start(start), end(end)
    {
        assert(start <= end);
    }

    explicit operator bool() const
    {
        return start < end;
    }

    // Comparator for storing ZRanges in a set/map.
    // Asserts that the two ZRanges are either disjoint or are equal.
    bool operator<(const ZRange& other) const
    {
        assert(end <= other.start || other.end <= start || 
            std::tie(start, end) == std::tie(other.start, other.end));
        return std::tie(start, end) < std::tie(other.start, other.end);
    }
};

// Represents a partition of the set of integers containing half-open intervals.
struct ZPartition {
    std::set<uint32_t> points = {0};

    ZPartition() {}

    ZPartition(const std::initializer_list<ZRange>& ranges)
    {
        for (const auto& ran: ranges) {
            insert(ran);
        }
    }

    void insert(const ZRange& ran)
    {
        if (ran) {
            points.insert(ran.start);
            if (ran.end < std::numeric_limits<uint32_t>::max()) {
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
        std::set<uint32_t>::const_iterator it;
        std::set<uint32_t>::const_iterator jt;
        std::set<uint32_t>::const_iterator end;

    public:
        Iterator(
            const std::set<uint32_t>& points,
            std::set<uint32_t>::const_iterator it)
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
            uint32_t a = std::numeric_limits<uint32_t>::max();
            uint32_t b = a;
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

}
